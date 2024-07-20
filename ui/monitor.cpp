/**
 * @file monitor.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "monitor.h"
#include "ui_monitor.h"
#include <qdebug.h>
#include <qglobal.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qlineedit.h>
#include <qmath.h>
#include <qpushbutton.h>

Monitor::Monitor( QWidget* parent ) : QWidget( parent ), ui( new Ui::Monitor ) {
    ui->setupUi( this );
    ui->horizontalLayout->insertWidget( 0, &osc );
    ui->horizontalLayout->setStretchFactor( &osc, 1 );
    connect( &update_data_timer_, SIGNAL( timeout() ), this, SLOT( update_data() ) );  // 测试用途的定时器
    update_data_timer_.start( 2 );

    // // 生成测试数据
    // double amplitude  = 10.0;
    // double frequency  = 1.0;
    // double phase      = 0.0;
    // double duration   = 10000.0;
    // double sampleRate = 100.0;
    // sine_wave_generator_.set_generator_parameters( amplitude, frequency, phase, sampleRate, duration );
    // sine_wave_generator_.generate_data( osc.out_sine_wave_data_ );

    // double amplitude2  = 20.0;
    // double frequency2  = 2.0;
    // double phase2      = 0.0;
    // double duration2   = 10000.0;
    // double sampleRate2 = 100.0;
    // square_wave_generator_.set_generator_parameters( amplitude2, frequency2, phase2, sampleRate2, duration2 );
    // square_wave_generator_.generate_data( osc.out_square_wave_data_ );
}

Monitor::~Monitor() {
    delete ui;
}

void Monitor::on_pushButton_add_variable_clicked() {
    if ( !osc.variables_.contains( ui->comboBox_monitor_var->currentText() ) ) {

        osc.variables_[ ui->comboBox_monitor_var->currentText() ].line_style_    = line_style_map_.value( ui->comboBox_line_type->currentText() );
        osc.variables_[ ui->comboBox_monitor_var->currentText() ].scatter_shape_ = scatter_shape_map_.value( ui->comboBox_dot_type->currentText() );
        osc.variables_[ ui->comboBox_monitor_var->currentText() ].color_         = color_map_.value( ui->comboBox_line_color->currentText() );
        osc.variables_[ ui->comboBox_monitor_var->currentText() ].line_width_    = qSqrt( ui->horizontalSlider_line_width->value() );

        osc.add_monitor_variable( ui->comboBox_monitor_var->currentText() );

        ui->comboBox_exist_variable->addItem( ui->comboBox_monitor_var->currentText() );
    }
    else {
        QMessageBox::information( this, "提示", "该变量已被监视，请勿重复添加！" );
    }
}

void Monitor::on_pushButton_remove_variable_clicked() {
    QString t_var_name = ui->comboBox_exist_variable->currentText();
    if ( !t_var_name.isEmpty() ) {
        osc.remove_monitor_variable( t_var_name );
        ui->comboBox_exist_variable->removeItem( ui->comboBox_exist_variable->currentIndex() );
        // QDebug( QtDebugMsg ) << "删除变量" << t_var_name << "成功";
    }
}

void Monitor::on_comboBox_sign_type_activated( const QString& in_sign_type ) {
    clear_form_layout( ui->formLayout_parameter );
    if ( in_sign_type == "正弦波" ) {
        QLabel* t_label_amplitude   = new QLabel( "幅值" );
        QLabel* t_label_frequency   = new QLabel( "频率" );
        QLabel* t_label_phase       = new QLabel( "相位" );
        QLabel* t_label_sample_rate = new QLabel( "采样率" );
        QLabel* t_label_duration    = new QLabel( "持续时间" );

        QLineEdit* t_lineEdit_amplitude   = new QLineEdit();
        QLineEdit* t_lineEdit_frequency   = new QLineEdit();
        QLineEdit* t_lineEdit_phase       = new QLineEdit();
        QLineEdit* t_lineEdit_sample_rate = new QLineEdit();
        QLineEdit* t_lineEdit_duration    = new QLineEdit();

        ui->formLayout_parameter->addRow( t_label_amplitude, t_lineEdit_amplitude );
        ui->formLayout_parameter->addRow( t_label_frequency, t_lineEdit_frequency );
        ui->formLayout_parameter->addRow( t_label_phase, t_lineEdit_phase );
        ui->formLayout_parameter->addRow( t_label_sample_rate, t_lineEdit_sample_rate );
        ui->formLayout_parameter->addRow( t_label_duration, t_lineEdit_duration );
    }
    else if ( in_sign_type == "方波" ) {
        QLabel* t_label_amplitude   = new QLabel( "幅值" );
        QLabel* t_label_frequency   = new QLabel( "频率" );
        QLabel* t_label_phase       = new QLabel( "相位" );
        QLabel* t_label_sample_rate = new QLabel( "采样率" );
        QLabel* t_label_duration    = new QLabel( "持续时间" );

        QLineEdit* t_lineEdit_amplitude   = new QLineEdit();
        QLineEdit* t_lineEdit_frequency   = new QLineEdit();
        QLineEdit* t_lineEdit_phase       = new QLineEdit();
        QLineEdit* t_lineEdit_sample_rate = new QLineEdit();
        QLineEdit* t_lineEdit_duration    = new QLineEdit();

        ui->formLayout_parameter->addRow( t_label_amplitude, t_lineEdit_amplitude );
        ui->formLayout_parameter->addRow( t_label_frequency, t_lineEdit_frequency );
        ui->formLayout_parameter->addRow( t_label_phase, t_lineEdit_phase );
        ui->formLayout_parameter->addRow( t_label_sample_rate, t_lineEdit_sample_rate );
        ui->formLayout_parameter->addRow( t_label_duration, t_lineEdit_duration );
    }
    else if ( in_sign_type == "梯形波" ) {}

    QPushButton* t_button_generate_data = new QPushButton( "生成数据" );
    ui->formLayout_parameter->addRow( t_button_generate_data );
    connect( t_button_generate_data, SIGNAL( clicked() ), this, SLOT( generate_data() ) );
}

void Monitor::update_data() {
    if ( !osc.variables_.isEmpty() ) {
        for ( const auto& t_var_name : osc.variables_.keys() ) {
            qint64 t_index = osc.variables_.constFind( t_var_name )->index_;
            if ( t_var_name == "正弦波" ) {
                if ( !osc.out_sine_wave_data_.empty() && t_index < osc.out_sine_wave_data_.size() ) {
                    osc.variables_[ t_var_name ].points_.append( QPointF( t_index, osc.out_sine_wave_data_[ t_index ] ) );
                    // QDebug( QtDebugMsg ) << "正弦波" << osc.variables_[ t_var_name ].points_.size();
                }
            }
            else if ( t_var_name == "方波" ) {
                if ( !osc.out_square_wave_data_.empty() && t_index < osc.out_square_wave_data_.size() ) {
                    osc.variables_[ t_var_name ].points_.append( QPointF( t_index, osc.out_square_wave_data_[ t_index ] ) );
                    // QDebug( QtDebugMsg ) << "方波" << osc.variables_[ t_var_name ].points_.size();
                }
            }
            osc.variables_[ t_var_name ].index_++;
        }
    }
}

void Monitor::generate_data() {
    if ( ui->comboBox_sign_type->currentText() == "正弦波" ) {
        double amplitude  = ui->formLayout_parameter->itemAt( 1 )->widget()->property( "text" ).toDouble();
        double frequency  = ui->formLayout_parameter->itemAt( 3 )->widget()->property( "text" ).toDouble();
        double phase      = ui->formLayout_parameter->itemAt( 5 )->widget()->property( "text" ).toDouble();
        double sampleRate = ui->formLayout_parameter->itemAt( 7 )->widget()->property( "text" ).toDouble();
        double duration   = ui->formLayout_parameter->itemAt( 9 )->widget()->property( "text" ).toDouble();

        sine_wave_generator_.set_generator_parameters( amplitude, frequency, phase, sampleRate, duration );
        sine_wave_generator_.generate_data( osc.out_sine_wave_data_ );
    }
    else if ( ui->comboBox_sign_type->currentText() == "方波" ) {
        double amplitude  = ui->formLayout_parameter->itemAt( 1 )->widget()->property( "text" ).toDouble();
        double frequency  = ui->formLayout_parameter->itemAt( 3 )->widget()->property( "text" ).toDouble();
        double phase      = ui->formLayout_parameter->itemAt( 5 )->widget()->property( "text" ).toDouble();
        double sampleRate = ui->formLayout_parameter->itemAt( 7 )->widget()->property( "text" ).toDouble();
        double duration   = ui->formLayout_parameter->itemAt( 9 )->widget()->property( "text" ).toDouble();

        square_wave_generator_.set_generator_parameters( amplitude, frequency, phase, sampleRate, duration );
        square_wave_generator_.generate_data( osc.out_square_wave_data_ );
    }
}

/**
 * @brief 清空表单布局元素
 *
 * @param in_layout
 */
void Monitor::clear_form_layout( QFormLayout* in_layout ) {
    if ( !in_layout )
        return;

    while ( QLayoutItem* item = in_layout->takeAt( 0 ) ) {
        if ( QWidget* widget = item->widget() ) {
            delete widget;
        }
        delete item;
    }
}
