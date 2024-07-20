/**
 * @file oscilloscope.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include "data_generation.h"
#include "osc_plot.h"
#include "qcustomplot.h"
#include <QWidget>
#include <qglobal.h>
#include <qhash.h>
#include <qmap.h>
#include <qpoint.h>
#include <qvector.h>

class Variable {
public:
    QCPGraph::LineStyle           line_style_;
    QCPScatterStyle::ScatterShape scatter_shape_;
    QColor                        color_;
    qreal                         line_width_;

    QVector< QPointF > points_;
    QCPGraph*          graph_;
    qint64             index_;
};

class OscilloScope : public QWidget {
    Q_OBJECT

public:
    explicit OscilloScope( QWidget* parent = Q_NULLPTR );
    ~OscilloScope() = default;

    void add_monitor_variable( const QString& in_line_name );

    void remove_monitor_variable( const QString& in_line_name );

    std::vector< double > out_sine_wave_data_;
    std::vector< double > out_square_wave_data_;
    // qint64                               lastet_index_ = 0;
    // QHash< QString, QVector< QPointF > > points_hash_;
    // QHash< QString, QCPGraph* >          graphs_hash_;
    // QHash< QString, qint64 >             index_hash_;
    QHash< QString, Variable > variables_;

protected:
    OscPlot* osc_plot_;

    QTimer update_plot_timer_;

private:
    QCPRange           x_range_;
    qint64             sample_size_ = 1024;
    QVector< QPointF > points_;

    QVector< double > x_points_ = QVector< double >( sample_size_ );
    QVector< double > y_points_ = QVector< double >( sample_size_ );

    void init();
    void process_points( const QVector< QPointF >& in_points );
    void simplify_curve_points( const QVector< QPointF >& in_points, QVector< QPointF >& out_points, double in_epsilon );
    void auto_fit_screen( int in_lastest_x );

    void   simplifyCurve( const QVector< QPointF >& in_points, QVector< QPointF >& out_points, double in_epsilon );
    double pointLineDistance( const QPointF& in_point, const QPointF& in_line_start, const QPointF& in_line_end );

private slots:
    void update();
};

#endif  // OSCILLOSCOPE_H