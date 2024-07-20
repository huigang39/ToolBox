/**
 * @file osc_tracer.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef OSC_TRACER_H
#define OSC_TRACER_H

#include "qcustomplot.h"

/**
 * @brief 在图表中显示鼠标所在位置的x,y值的追踪显示器
 */
class OscTracer : public QObject {
    Q_OBJECT

public:
    enum TracerType {
        x_axis_tracer,  //依附在X轴上显示x值
        y_axis_tracer,  //依附在Y轴上显示y值
        data_tracer     //在图中显示x,y值
    };

    explicit OscTracer( QCustomPlot* in_plot, TracerType in_type, QObject* in_parent = Q_NULLPTR );
    ~OscTracer();
    void set_pen( const QPen& in_pen );
    void set_brush( const QBrush& in_brush );
    void set_text( const QString& in_text );
    void set_label_pen( const QPen& in_pen );
    void update_position( double in_x_xalue, double in_y_value );
    void set_visible( bool in_is_visible );

protected:
    bool           visible_;  //是否可见
    TracerType     type_;     //类型
    QCustomPlot*   plot_;     //图表
    QCPItemTracer* tracer_;   //跟踪的点
    QCPItemText*   label_;    //显示的数值
    QCPItemLine*   arrow_;    //箭头
};

/**
 * @brief 用于显示鼠标移动过程中的鼠标位置的直线
 *
 */
class TraceLine : public QObject {
public:
    enum LineType {
        VerticalLine,  //垂直线
        HorizonLine,   //水平线
        Both           //同时显示水平和垂直线
    };

    explicit TraceLine( QCustomPlot* in_plot, LineType in_type = VerticalLine, QObject* in_parent = Q_NULLPTR );
    ~TraceLine();
    void init_line();
    void update_position( double in_x_value, double in_y_yalue );

    void set_visible( bool in_is_visible ) {
        if ( line_v_ )
            line_v_->setVisible( in_is_visible );
        if ( line_h_ )
            line_h_->setVisible( in_is_visible );
    }

protected:
    bool                 is_visible_;  //是否可见
    LineType             type_;        //类型
    QCustomPlot*         plot_;        //图表
    QCPItemStraightLine* line_v_;      //垂直线
    QCPItemStraightLine* line_h_;      //水平线
};

#endif  // OSC_TRACER_H