/**
 * @file tool_box.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-14
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef TOOL_BOX_H
#define TOOL_BOX_H

#include "network_share.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ToolBox;
}
QT_END_NAMESPACE

class ToolBox : public QMainWindow {
    Q_OBJECT

public:
    explicit ToolBox( QWidget* parent = Q_NULLPTR );
    ~ToolBox() override;

private:
    Ui::ToolBox* ui;

    NetworkShare network_share_;
};

#endif  // TOOL_BOX_H
