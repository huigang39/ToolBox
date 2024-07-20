/**
 * @file network_share.h
 * @author Huigang Wang (huigang.wang@fftai.com)
 * @brief
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024 Huigang Wang
 *
 */

#ifndef NETWORK_SHARE_H
#define NETWORK_SHARE_H

#include <QProcess>
#include <qchar.h>
#include <qlist.h>
#include <qprocess.h>

class NetworkShare {
public:
    explicit NetworkShare();
    ~NetworkShare();

    void scan_network_adapter();
    void enable_network_share();
    void disable_network_share();
    void set_command( const QString& command, const QStringList& params );

private:
    QProcess    process_;
    QString     command_;
    QStringList params_;
};

#endif  // NETWORK_SHARE_H