/**
 * @file osc_plot.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "osc_plot.h"

OscPlot::OscPlot( QWidget* parent )
    : QCustomPlot( parent ), is_show_tracer_( false ), x_tracer_( Q_NULLPTR ), y_tracer_( Q_NULLPTR ), data_tracers_( QList< OscTracer* >() ), line_tracer_( Q_NULLPTR ) {

    setOpenGl( true );

    QDebug( QtMsgType::QtInfoMsg ) << "OpenGL is open " << openGl();
}

void OscPlot::mouseMoveEvent( QMouseEvent* event ) {
    QCustomPlot::mouseMoveEvent( event );

    if ( is_left_button_pressed_ ) {
        is_paused_scroll_ = true;
    }

    if ( is_show_tracer_ ) {
        //当前鼠标位置（像素坐标）
        int x_pos = event->pos().x();
        int y_pos = event->pos().y();

        //像素坐标转成实际的x,y轴的坐标
        float x_val = this->xAxis->pixelToCoord( x_pos );
        float y_val = this->yAxis->pixelToCoord( y_pos );

        if ( Q_NULLPTR == x_tracer_ ) {
            x_tracer_ = new OscTracer( this, OscTracer::x_axis_tracer );  // x轴
        }
        x_tracer_->update_position( x_val, y_val );

        if ( Q_NULLPTR == y_tracer_ ) {
            y_tracer_ = new OscTracer( this, OscTracer::y_axis_tracer );  // y轴
        }
        y_tracer_->update_position( x_val, y_val );

        int nTracerCount = data_tracers_.count();
        int nGraphCount  = graphCount();
        if ( nTracerCount < nGraphCount ) {
            for ( int i = nTracerCount; i < nGraphCount; ++i ) {
                OscTracer* tracer = new OscTracer( this, OscTracer::data_tracer );
                data_tracers_.append( tracer );
            }
        }
        else if ( nTracerCount > nGraphCount ) {
            for ( int i = nGraphCount; i < nTracerCount; ++i ) {
                OscTracer* tracer = data_tracers_[ i ];
                if ( tracer ) {
                    tracer->set_visible( false );
                }
            }
        }

        if ( Q_NULLPTR == line_tracer_ ) {
            line_tracer_ = new TraceLine( this, TraceLine::Both );  //直线
        }
        line_tracer_->update_position( x_val, y_val );

        this->replot();  //曲线重绘
    }
}

void OscPlot::mousePressEvent( QMouseEvent* event ) {
    QCustomPlot::mousePressEvent( event );

    if ( event->button() == Qt::LeftButton ) {
        is_show_tracer_         = !is_show_tracer_;
        is_left_button_pressed_ = true;
    }
    else if ( event->button() == Qt::RightButton ) {
        is_stop_plot_ = !is_stop_plot_;
    }
    else if ( event->button() == Qt::MiddleButton ) {
        is_paused_scroll_ = false;
        is_auto_fit_axis_ = true;
    }

    show_tracer( is_show_tracer_ );
}

void OscPlot::mouseReleaseEvent( QMouseEvent* event ) {
    QCustomPlot::mouseReleaseEvent( event );

    if ( event->button() == Qt::LeftButton ) {
        is_left_button_pressed_ = false;
    }
}

void OscPlot::wheelEvent( QWheelEvent* event ) {
    QCustomPlot::wheelEvent( event );

    is_auto_fit_axis_ = false;

    if ( event->angleDelta().y() > 0 ) {
        // 向上滚动，缩小X轴范围
        x_axis_range_ *= 0.9;
    }
    else {
        // 向下滚动，增大X轴范围
        x_axis_range_ *= 1.1;
    }
}
