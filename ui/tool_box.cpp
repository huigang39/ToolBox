/**
 * @file tool_box.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-14
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "tool_box.h"
// #include "monitor.h"
#include "network_share.h"
#include "ui_tool_box.h"

ToolBox::ToolBox( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::ToolBox ) {
    ui->setupUi( this );

    // Monitor*     t_monitor         = new Monitor();
    // QVBoxLayout* t_vertical_layout = new QVBoxLayout();
    // t_vertical_layout->addWidget( t_monitor );
    // ui->centralwidget->setLayout( t_vertical_layout );

    network_share_.scan_network_adapter();
}

ToolBox::~ToolBox() {
    delete ui;
}
