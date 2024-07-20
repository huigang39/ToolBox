/**
 * @file osc_tracer.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "osc_tracer.h"

OscTracer::OscTracer( QCustomPlot* in_plot, TracerType in_type, QObject* in_parent ) : QObject( in_parent ), plot_( in_plot ), type_( in_type ) {
    visible_ = true;
    tracer_  = Q_NULLPTR;  // 跟踪的点
    label_   = Q_NULLPTR;  // 显示的数值
    arrow_   = Q_NULLPTR;  // 箭头
    if ( plot_ ) {
        QColor t_color_default( Qt::red );
        QBrush t_brush_default( Qt::NoBrush );
        QPen   t_pen_default( t_color_default );
        t_pen_default.setWidthF( 0.5 );

        tracer_ = new QCPItemTracer( plot_ );
        tracer_->setStyle( QCPItemTracer::tsCircle );
        tracer_->setPen( t_pen_default );
        tracer_->setBrush( t_brush_default );

        label_ = new QCPItemText( plot_ );
        label_->setLayer( "overlay" );
        label_->setClipToAxisRect( false );
        label_->setPadding( QMargins( 5, 5, 5, 5 ) );
        label_->setBrush( t_brush_default );
        label_->setPen( t_pen_default );
        label_->position->setParentAnchor( tracer_->position );
        //        m_label->setFont(QFont("宋体", 8));
        label_->setFont( QFont( "Arial", 8 ) );
        label_->setColor( t_color_default );
        label_->setText( "" );

        arrow_ = new QCPItemLine( plot_ );
        QPen arrowPen( t_color_default, 1 );
        arrow_->setPen( t_pen_default );
        arrow_->setLayer( "overlay" );
        arrow_->setClipToAxisRect( false );
        arrow_->setHead( QCPLineEnding::esSpikeArrow );  //设置头部为箭头形状

        switch ( type_ ) {
        case x_axis_tracer: {
            tracer_->position->setTypeX( QCPItemPosition::ptPlotCoords );
            tracer_->position->setTypeY( QCPItemPosition::ptAxisRectRatio );
            tracer_->setSize( 7 );
            label_->setPositionAlignment( Qt::AlignTop | Qt::AlignHCenter );

            arrow_->end->setParentAnchor( tracer_->position );
            arrow_->start->setParentAnchor( arrow_->end );
            arrow_->start->setCoords( 0, 20 );  //偏移量
            break;
        }
        case y_axis_tracer: {
            tracer_->position->setTypeX( QCPItemPosition::ptAxisRectRatio );
            tracer_->position->setTypeY( QCPItemPosition::ptPlotCoords );
            tracer_->setSize( 7 );
            label_->setPositionAlignment( Qt::AlignRight | Qt::AlignHCenter );

            arrow_->end->setParentAnchor( tracer_->position );
            arrow_->start->setParentAnchor( label_->position );
            arrow_->start->setCoords( -20, 0 );  //偏移量
            break;
        }
        case data_tracer: {
            tracer_->position->setTypeX( QCPItemPosition::ptPlotCoords );
            tracer_->position->setTypeY( QCPItemPosition::ptPlotCoords );
            tracer_->setSize( 5 );

            label_->setPositionAlignment( Qt::AlignLeft | Qt::AlignVCenter );

            arrow_->end->setParentAnchor( tracer_->position );
            arrow_->start->setParentAnchor( arrow_->end );
            arrow_->start->setCoords( 20, 0 );
            break;
        }
        default:
            break;
        }
        set_visible( false );
    }
}

OscTracer::~OscTracer() {
    if ( plot_ ) {
        if ( tracer_ ) {
            plot_->removeItem( tracer_ );
        }
        if ( label_ ) {
            plot_->removeItem( label_ );
        }
        if ( arrow_ ) {
            plot_->removeItem( arrow_ );
        }
    }
}

void OscTracer::set_pen( const QPen& in_pen ) {
    if ( tracer_ ) {
        tracer_->setPen( in_pen );
    }
    if ( arrow_ ) {
        arrow_->setPen( in_pen );
    }
}

void OscTracer::set_brush( const QBrush& in_brush ) {
    if ( tracer_ ) {
        tracer_->setBrush( in_brush );
    }
}

void OscTracer::set_label_pen( const QPen& in_pen ) {
    if ( label_ ) {
        label_->setPen( in_pen );
        label_->setBrush( Qt::NoBrush );
        label_->setColor( in_pen.color() );
    }
}

void OscTracer::set_text( const QString& in_text ) {
    if ( label_ ) {
        label_->setText( in_text );
    }
}

void OscTracer::set_visible( bool in_is_visible ) {
    visible_ = in_is_visible;
    if ( tracer_ ) {
        tracer_->setVisible( visible_ );
    }
    if ( label_ ) {
        label_->setVisible( visible_ );
    }
    if ( arrow_ ) {
        arrow_->setVisible( visible_ );
    }
}

void OscTracer::update_position( double x_value, double y_value ) {
    if ( !visible_ ) {
        set_visible( true );
        visible_ = true;
    }

    if ( y_value > plot_->yAxis->range().upper ) {
        y_value = plot_->yAxis->range().upper;
    }

    switch ( type_ ) {
    case x_axis_tracer: {
        tracer_->position->setCoords( x_value, 1 );
        label_->position->setCoords( 0, 15 );
        arrow_->start->setCoords( 0, 15 );
        arrow_->end->setCoords( 0, 0 );
        set_text( QString::number( x_value ) );
        break;
    }
    case y_axis_tracer: {
        tracer_->position->setCoords( 0, y_value );
        label_->position->setCoords( -20, 0 );
        //        m_arrow->start->setCoords(20, 0);
        //        m_arrow->end->setCoords(0, 0);
        set_text( QString::number( y_value ) );
        break;
    }
    case data_tracer: {
        tracer_->position->setCoords( x_value, y_value );
        label_->position->setCoords( 20, 0 );
        set_text( QString( "x:%1,y:%2" ).arg( x_value ).arg( y_value ) );
        break;
    }
    default:
        break;
    }
}

TraceLine::TraceLine( QCustomPlot* in_plot, LineType in_type, QObject* in_parent ) : QObject( in_parent ), type_( in_type ), plot_( in_plot ) {
    line_v_ = Q_NULLPTR;
    line_h_ = Q_NULLPTR;
    init_line();
}

TraceLine::~TraceLine() {
    if ( plot_ ) {
        if ( line_v_ ) {
            plot_->removeItem( line_v_ );
        }
        if ( line_h_ ) {
            plot_->removeItem( line_h_ );
        }
    }
}

void TraceLine::init_line() {
    if ( plot_ ) {
        QPen t_lines_pen( Qt::red, 1, Qt::SolidLine );

        if ( VerticalLine == type_ || Both == type_ ) {
            line_v_ = new QCPItemStraightLine( plot_ );  //垂直线
            line_v_->setLayer( "overlay" );
            line_v_->setPen( t_lines_pen );
            line_v_->setClipToAxisRect( true );
            line_v_->point1->setCoords( 0, 0 );
            line_v_->point2->setCoords( 0, 0 );
        }

        if ( HorizonLine == type_ || Both == type_ ) {
            line_h_ = new QCPItemStraightLine( plot_ );  //水平线
            line_h_->setLayer( "overlay" );
            line_h_->setPen( t_lines_pen );
            line_h_->setClipToAxisRect( true );
            line_h_->point1->setCoords( 0, 0 );
            line_h_->point2->setCoords( 0, 0 );
        }
    }
}

void TraceLine::update_position( double x_value, double y_value ) {
    if ( VerticalLine == type_ || Both == type_ ) {
        if ( line_v_ ) {
            line_v_->point1->setCoords( x_value, plot_->yAxis->range().lower );
            line_v_->point2->setCoords( x_value, plot_->yAxis->range().upper );
        }
    }

    if ( HorizonLine == type_ || Both == type_ ) {
        if ( line_h_ ) {
            line_h_->point1->setCoords( plot_->xAxis->range().lower, y_value );
            line_h_->point2->setCoords( plot_->xAxis->range().upper, y_value );
        }
    }
}