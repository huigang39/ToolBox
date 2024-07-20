/**
 * @file osc_plot.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef OSC_PLOT_H
#define OSC_PLOT_H

#include "osc_tracer.h"
#include "qcustomplot.h"
#include <QList>

class OscPlot : public QCustomPlot {
    Q_OBJECT

public:
    OscPlot( QWidget* parent = 0 );

    bool   is_paused_scroll_       = false;
    bool   is_auto_fit_axis_       = true;   // 自适应屏幕
    bool   is_stop_plot_           = false;  // 是否停止绘图
    bool   is_left_button_pressed_ = false;
    double default_x_axis_range_   = 1024;
    double x_axis_range_           = default_x_axis_range_;

protected:
    void mouseMoveEvent( QMouseEvent* event ) override;
    void mousePressEvent( QMouseEvent* event ) override;
    void mouseReleaseEvent( QMouseEvent* event ) override;
    void wheelEvent( QWheelEvent* event ) override;

public:
    /**
     * @brief 显示鼠标追踪器
     * @param in_is_show 是否显示
     * @return void
     */
    void show_tracer( bool in_is_show ) {
        is_show_tracer_ = in_is_show;
        if ( x_tracer_ ) {
            x_tracer_->set_visible( is_show_tracer_ );
        }
        foreach ( OscTracer* tracer, data_tracers_ ) {
            if ( tracer ) {
                tracer->set_visible( is_show_tracer_ );
            }
        }
        if ( line_tracer_ ) {
            line_tracer_->set_visible( is_show_tracer_ );
        }
    }

    /**
     * @brief 是否显示鼠标追踪器
     * @param  void
     * @return true/false
     */
    bool is_show_tracer() {
        return is_show_tracer_;
    };

private:
    bool                is_show_tracer_;  // 是否显示追踪器（鼠标在图中移动，显示对应的值）
    OscTracer*          x_tracer_;        // X 轴
    OscTracer*          y_tracer_;        // Y 轴
    QList< OscTracer* > data_tracers_;    //
    TraceLine*          line_tracer_;     // 直线
};

#endif  // OSC_PLOT_H