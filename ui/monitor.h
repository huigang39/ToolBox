/**
 * @file monitor.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "oscilloscope.h"
#include "qcustomplot.h"
#include <QWidget>
#include <qmap.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Monitor;
}
QT_END_NAMESPACE

class Monitor : public QWidget {
    Q_OBJECT

public:
    explicit Monitor( QWidget* parent = Q_NULLPTR );
    ~Monitor() override;

private:
    Ui::Monitor* ui;

    OscilloScope osc;

    QTimer update_data_timer_;

    SineWaveGenerator      sine_wave_generator_;
    SquareWaveGenerator    square_wave_generator_;
    TrapezoidWaveGenerator trapezoid_wave_generator_;

    const QMap< QString, QCPGraph::LineStyle > line_style_map_ = {
        { "直线", QCPGraph::lsLine }, { "左侧步长", QCPGraph::lsStepLeft }, { "右侧步长", QCPGraph::lsStepRight }, { "中间步长", QCPGraph::lsStepCenter }, { "脉冲", QCPGraph::lsImpulse },
    };

    const QMap< QString, QCPScatterStyle::ScatterShape > scatter_shape_map_ = {
        { "无", QCPScatterStyle::ssNone },     { "单点", QCPScatterStyle::ssDot },  { "十字", QCPScatterStyle::ssPlus },
        { "圆形", QCPScatterStyle::ssCircle }, { "星形", QCPScatterStyle::ssStar }, { "三角形", QCPScatterStyle::ssTriangle },
    };

    const QMap< QString, QColor > color_map_ = {
        { "红", QColor( 255, 0, 0 ) }, { "绿", QColor( 0, 255, 0 ) }, { "蓝", QColor( 0, 0, 255 ) }, { "黄", QColor( 255, 255, 0 ) }, { "紫", QColor( 255, 0, 255 ) },
    };

    void clear_form_layout( QFormLayout* in_layout );

public slots:
    void update_data();

    void on_pushButton_add_variable_clicked();
    void on_pushButton_remove_variable_clicked();
    void generate_data();

    void on_comboBox_sign_type_activated( const QString& in_sign_type );
};

#endif  // MONITOR_H
