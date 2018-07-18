/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef RATECONTROLLER_H
#define RATECONTROLLER_H

#include <QObject>
#include <QSet>
#include <QTime>

class PeerWireClient;

class RateController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( qint64 uploadLimit   MEMBER m_uploadLimit   NOTIFY uploadLimitChanged )
    Q_PROPERTY( qint64 downloadLimit MEMBER m_downloadLimit NOTIFY downloadLimitChanged )

public:
    RateController(QObject *parent = nullptr);
    ~RateController();
    static RateController *instance();

    void    addSocket(PeerWireClient *socket);
    void    removeSocket(PeerWireClient *socket);

    qint64  uploadLimit() const;
    qint64  downloadLimit() const;
    void    setUploadLimit(qint64 bytesPerSecond);
    void    setDownloadLimit(qint64 bytesPerSecond);

signals:
    void uploadLimitChanged  ( qint64 newLimit );
    void downloadLimitChanged( qint64 newLimit );

public slots:
    void transfer();
    void scheduleTransfer();

private:
    bool                    transferScheduled;
    QTime                   stopWatch;

    QSet<PeerWireClient *>  sockets;

    qint64                  m_uploadLimit;
    qint64                  m_downloadLimit;
};

/// Wrapper for Qml Singleton using.
/// QQmlEngine object is owner for any singletons in qml
/// but RateController is owned by QGlobalFactory
/// so, RateControllerQml dectructor is not destruct RateController
class RateControllerQml : public QObject
{
    Q_OBJECT
    Q_PROPERTY( qint64 uploadLimit   READ uploadLimit   WRITE setUploadLimit   NOTIFY uploadLimitChanged )
    Q_PROPERTY( qint64 downloadLimit READ downloadLimit WRITE setDownloadLimit NOTIFY downloadLimitChanged )

public:
    RateControllerQml()
    {
        auto rc = RateController::instance();
        connect( rc, &RateController::uploadLimitChanged, this, &RateControllerQml::uploadLimitChanged );
        connect( rc, &RateController::downloadLimitChanged, this, &RateControllerQml::downloadLimitChanged );
    }

    qint64  uploadLimit() const{
        return RateController::instance()->uploadLimit();
    }
    qint64  downloadLimit() const{
        return RateController::instance()->downloadLimit();
    }
    void    setUploadLimit(qint64 bytesPerSecond){
        RateController::instance()->setUploadLimit( bytesPerSecond );
    }
    void    setDownloadLimit(qint64 bytesPerSecond){
        RateController::instance()->setDownloadLimit( bytesPerSecond );
    }

signals:
    void uploadLimitChanged  ( qint64 newLimit );
    void downloadLimitChanged( qint64 newLimit );
};

#endif
