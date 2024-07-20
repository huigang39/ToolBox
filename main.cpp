/**
 * @file main.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-14
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "tool_box.h"
#include <QApplication>

int main( int argc, char* argv[] ) {
    QApplication app( argc, argv );
    ToolBox      tool_box;
    tool_box.show();
    return QApplication::exec();
}
