/**
 * @file oscilloscope.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "oscilloscope.h"
#include "qcustomplot.h"
#include <algorithm>

OscilloScope::OscilloScope( QWidget* parent ) : QWidget( parent ) {
    init();
}

void OscilloScope::init() {
    // 设置窗口属性和标题
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;  // 允许最小化和最大化按钮
    flags |= Qt::WindowCloseButtonHint;    // 允许关闭按钮
    setWindowFlags( flags );
    const QString strName = "性能测试";
    setWindowTitle( "QCustomplot " + strName );  // 设置窗口标题

    // 初始化OscPlot对象
    osc_plot_ = new OscPlot();
    osc_plot_->show_tracer( false );  // 不显示追踪器

    // 配置图表布局和标题
    osc_plot_->plotLayout()->insertRow( 0 );
    osc_plot_->plotLayout()->addElement( 0, 0, new QCPTextElement( osc_plot_, strName, QFont( "微软雅黑", 12, QFont::Bold ) ) );

    // 设置图例可见并配置
    osc_plot_->legend->setVisible( true );
    QFont t_legend_font = font();
    t_legend_font.setPointSize( 9 );
    osc_plot_->legend->setFont( t_legend_font );
    osc_plot_->legend->setBrush( QBrush( QColor( 255, 255, 255, 230 ) ) );

    // 设置图表内边距对齐方式
    osc_plot_->axisRect()->insetLayout()->setInsetAlignment( 0, Qt::AlignTop | Qt::AlignRight );

    // 连接信号和槽以同步x轴和y轴的范围
    connect( osc_plot_->xAxis, SIGNAL( rangeChanged( QCPRange ) ), osc_plot_->xAxis2, SLOT( setRange( QCPRange ) ) );
    connect( osc_plot_->yAxis, SIGNAL( rangeChanged( QCPRange ) ), osc_plot_->yAxis2, SLOT( setRange( QCPRange ) ) );

    // 设置图表交互
    osc_plot_->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );

    // 设置x轴和y轴标签
    osc_plot_->xAxis->setLabel( "采样数" );
    osc_plot_->yAxis->setLabel( "幅值" );

    // 设置x轴范围并自动缩放轴
    osc_plot_->xAxis->setRange( 0, osc_plot_->default_x_axis_range_ );
    osc_plot_->rescaleAxes( true );

    // 重新绘制图表
    osc_plot_->replot();

    // 设置布局并添加osc_plot_控件
    QVBoxLayout* t_layout = new QVBoxLayout;
    t_layout->addWidget( osc_plot_ );
    setLayout( t_layout );

    // 设置窗口大小
    resize( QSize( 1000, 600 ) );

    connect( &update_plot_timer_, SIGNAL( timeout() ), this, SLOT( update() ) );  // 更新图表的定时器
    update_plot_timer_.start( 30 );
}

/**
 * @brief 刷新绘图
 *
 */
void OscilloScope::update() {
    if ( !variables_.isEmpty() ) {
        qint64 t_lastest_x = 0;
        for ( const auto& t_var_name : variables_.keys() ) {
            if ( !osc_plot_->is_stop_plot_ ) {
                if ( !variables_.constFind( t_var_name ).value().points_.isEmpty() ) {
                    variables_.value( t_var_name ).graph_->data().data()->clear();

                    points_ = variables_.value( t_var_name ).points_;

                    x_points_.clear();
                    y_points_.clear();

                    process_points( points_ );

                    variables_.value( t_var_name ).graph_->addData( x_points_, y_points_, true );

                    // QDebug( QtMsgType::QtInfoMsg ) << "xPoints.size():" << x_points_.size() << "back(): " << x_points_.back();
                    QDebug( QtMsgType::QtInfoMsg ) << "X back(): " << points_.back().x() << "Y back(): " << points_.back().y();
                }

                // 找出所有监视变量中最大的x坐标
                if ( t_lastest_x < points_.back().x() ) {
                    t_lastest_x = points_.back().x();
                }

                auto_fit_screen( t_lastest_x );
                osc_plot_->replot( QCustomPlot::rpQueuedReplot );
            }
            else {
                variables_.find( t_var_name )->points_.clear();
            }
        }
    }
}

/**
 * @brief 自适应屏幕
 *
 * @param in_index 最新的数据点索引
 */
void OscilloScope::auto_fit_screen( int in_lastest_x ) {
    if ( !osc_plot_->is_paused_scroll_ ) {
        if ( osc_plot_->is_auto_fit_axis_ ) {
            osc_plot_->rescaleAxes();
            QCPRange t_current_y_range = osc_plot_->yAxis->range();
            double   t_range_size      = t_current_y_range.upper - t_current_y_range.lower;
            double   t_margin          = t_range_size * 0.05;
            osc_plot_->yAxis->setRange( t_current_y_range.lower - t_margin, t_current_y_range.upper + t_margin );
        }
        if ( in_lastest_x > osc_plot_->default_x_axis_range_ ) {
            // osc_plot_->xAxis->setRange( in_lastest_x - osc_plot_->default_x_axis_range_, in_lastest_x );
            // osc_plot_->x_axis_range_ = osc_plot_->default_x_axis_range_;
            osc_plot_->xAxis->setRange( in_lastest_x - osc_plot_->x_axis_range_, in_lastest_x );
        }
        else {
            osc_plot_->xAxis->setRange( 0, in_lastest_x );
        }
    }
}

/**
 * @brief 添加监视变量
 *
 * @param in_line_name 线条名称
 */
void OscilloScope::add_monitor_variable( const QString& in_line_name ) {
    QCPGraph::LineStyle           t_line_style    = variables_.value( in_line_name ).line_style_;
    QCPScatterStyle::ScatterShape t_scatter_shape = variables_.value( in_line_name ).scatter_shape_;
    QColor                        t_color         = variables_.value( in_line_name ).color_;
    qreal                         t_line_width    = variables_.value( in_line_name ).line_width_;

    variables_[ in_line_name ].graph_ = osc_plot_->addGraph();

    variables_[ in_line_name ].graph_->setName( in_line_name );
    variables_[ in_line_name ].graph_->setLineStyle( t_line_style );
    variables_[ in_line_name ].graph_->setScatterStyle( t_scatter_shape );
    variables_[ in_line_name ].graph_->setPen( QPen( t_color, t_line_width ) );

    // points_hash_.insert( in_line_name, QVector< QPointF >() );
    // graphs_hash_.insert( in_line_name, t_graph );
    // index_hash_.insert( in_line_name, 0 );
}

void OscilloScope::remove_monitor_variable( const QString& in_line_name ) {
    // if ( points_hash_.contains( in_line_name ) ) {
    //     graphs_hash_.value( in_line_name )->data().data()->clear();
    //     graphs_hash_.value( in_line_name )->removeFromLegend();
    //     graphs_hash_.remove( in_line_name );
    //     index_hash_.remove( in_line_name );
    //     points_hash_.remove( in_line_name );
    //     osc_plot_->replot();
    // }

    if ( variables_.contains( in_line_name ) ) {
        variables_.value( in_line_name ).graph_->data().data()->clear();
        variables_.value( in_line_name ).graph_->removeFromLegend();
        variables_.remove( in_line_name );
        osc_plot_->replot();
    }
}

// Douglas-Peucker 算法实现
void OscilloScope::simplifyCurve( const QVector< QPointF >& in_points, QVector< QPointF >& out_points, double in_epsilon ) {
    if ( in_points.size() < 3 ) {
        out_points = in_points;
        return;
    }

    // 查找距离首尾直线最远的点
    int    t_index       = -1;
    double t_maxDistance = 0.0;
    for ( int i = 1; i < in_points.size() - 1; ++i ) {
        double distance = pointLineDistance( in_points[ i ], in_points.first(), in_points.last() );
        if ( distance > t_maxDistance ) {
            t_index       = i;
            t_maxDistance = distance;
        }
    }

    // 如果最大距离小于epsilon，则认为是一条直线
    if ( t_maxDistance > in_epsilon ) {
        // 分别对左右两部分进行递归简化
        QVector< QPointF > leftPoints, rightPoints;
        for ( int i = 0; i <= t_index; ++i )
            leftPoints.append( in_points[ i ] );
        for ( int i = t_index; i < in_points.size(); ++i )
            rightPoints.append( in_points[ i ] );

        QVector< QPointF > leftSimplified, rightSimplified;
        simplifyCurve( leftPoints, leftSimplified, in_epsilon );
        simplifyCurve( rightPoints, rightSimplified, in_epsilon );

        // 合并结果，注意去除重复的中间点
        out_points = leftSimplified;
        rightSimplified.removeFirst();
        out_points += rightSimplified;
    }
    else {
        // 如果是一条直线，则只保留首尾两点
        out_points.clear();
        out_points.append( in_points.first() );
        out_points.append( in_points.last() );
    }
}

// 计算点到线段的距离
double OscilloScope::pointLineDistance( const QPointF& in_point, const QPointF& in_line_start, const QPointF& in_line_end ) {
    double t_normal_length =
        sqrt( ( in_line_end.x() - in_line_start.x() ) * ( in_line_end.x() - in_line_start.x() ) + ( in_line_end.y() - in_line_start.y() ) * ( in_line_end.y() - in_line_start.y() ) );
    return fabs( ( in_point.x() - in_line_start.x() ) * ( in_line_end.y() - in_line_start.y() ) - ( in_point.y() - in_line_start.y() ) * ( in_line_end.x() - in_line_start.x() ) ) / t_normal_length;
}

void OscilloScope::process_points( const QVector< QPointF >& in_points ) {
    // 提取x坐标点用于查找
    QVector< double > t_x_coords;
    for ( const auto& point : in_points ) {
        t_x_coords.append( point.x() );
    }

    // 查找在当前x轴显示范围内的点的索引
    x_range_       = osc_plot_->xAxis->range();
    int startIndex = std::lower_bound( t_x_coords.begin(), t_x_coords.end(), x_range_.lower ) - t_x_coords.begin();
    int endIndex   = std::upper_bound( t_x_coords.begin(), t_x_coords.end(), x_range_.upper ) - t_x_coords.begin();

    // 计算需要抽样的数据点数量
    int totalPoints = endIndex - startIndex;
    if ( totalPoints > sample_size_ ) {
        // 计算每个抽样间隔
        int interval = totalPoints / sample_size_;

        for ( int i = startIndex; i < endIndex; i += interval ) {
            // 保留每个区间的起始点
            x_points_.append( in_points[ i ].x() );
            y_points_.append( in_points[ i ].y() );

            // 查找并保留每个区间内的极值点
            double t_min_x = in_points[ i ].x(), t_max_x = in_points[ i ].x();
            double t_min_y = in_points[ i ].y(), t_max_y = in_points[ i ].y();
            for ( int j = i; j < i + interval && j < endIndex; ++j ) {
                if ( in_points[ j ].y() < t_min_y ) {
                    t_min_x = in_points[ j ].x();
                    t_min_y = in_points[ j ].y();
                }
                if ( in_points[ j ].y() > t_max_y ) {
                    t_max_x = in_points[ j ].x();
                    t_max_y = in_points[ j ].y();
                }
            }
            // 避免重复添加起始点
            if ( t_min_x != x_points_.last() ) {
                x_points_.append( t_min_x );
                y_points_.append( t_min_y );
            }
            if ( t_max_x != x_points_.last() && t_max_x != t_min_x ) {
                x_points_.append( t_max_x );
                y_points_.append( t_max_y );
            }
        }
    }
    else {
        // 提取对应的x和y坐标点
        for ( int i = startIndex; i < endIndex && i < in_points.size(); ++i ) {
            x_points_.append( in_points[ i ].x() );
            y_points_.append( in_points[ i ].y() );
        }
    }
}

// TODO: 待实现
void OscilloScope::simplify_curve_points( const QVector< QPointF >& in_points, QVector< QPointF >& out_points, double in_epsilon ) {
    QVector< QPointF > t_simplified_points;
    simplifyCurve( in_points, t_simplified_points, in_epsilon );
    out_points = t_simplified_points;
}