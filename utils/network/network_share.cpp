/**
 * @file network_share.cpp
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#include "network_share.h"
#include <QDebug>

void NetworkShare::scan_network_adapter() {
    command_ = "powershell.exe";
    params_ << "-NoProfile" << "-Command" << "Get-NetAdapter";

    set_command( command_, params_ );
}

void NetworkShare::enable_network_share() {
    // Enable network share
}

void NetworkShare::disable_network_share() {
    // Disable network share
}

void NetworkShare::set_command( const QString& command, const QStringList& params ) {
    process_.start( command, params );

    if ( !process_.waitForStarted() ) {
        QDebug( QtWarningMsg ) << "Failed to start process.";
        return;
    }

    QString t_output = process_.readAllStandardOutput();
    QDebug( QtWarningMsg ) << t_output;

    QString t_error = process_.readAllStandardError();
    if ( !t_error.isEmpty() ) {
        QDebug( QtWarningMsg ) << t_error;
    }
}