/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

 #ifndef DEVICE_H
 #define DEVICE_H

#include <QObject>

#include <cmath>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QScreen>
#include <QTouchDevice>
#include <QStandardPaths>
#include "QQmlConstRefPropertyHelpers.h"
#include <QSysInfo>

class Device : public QObject {
    Q_OBJECT

    Q_PROPERTY(FormFactor formFactor READ formFactor NOTIFY geometryChanged)
    Q_PROPERTY(QString name READ name NOTIFY geometryChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY geometryChanged)

    Q_PROPERTY(bool isPortrait READ isPortrait NOTIFY geometryChanged)
    Q_PROPERTY(bool isMobile READ isMobile CONSTANT)
    Q_PROPERTY(bool hasTouchScreen READ hasTouchScreen CONSTANT)
    Q_PROPERTY(bool hoverEnabled READ hoverEnabled CONSTANT)
    Q_PROPERTY(int gridUnit READ gridUnit NOTIFY geometryChanged)


    // Standered Paths
    QML_READONLY_CSTREF_PROPERTY( QString, appDir )
    QML_READONLY_CSTREF_PROPERTY( QString, appConfigDir )
    QML_READONLY_CSTREF_PROPERTY( QString, dataDir )
    QML_READONLY_CSTREF_PROPERTY( QString, downloadsDir )
    QML_READONLY_CSTREF_PROPERTY( QString, homeDir )
    QML_READONLY_CSTREF_PROPERTY( QString, picturesDir )
    QML_READONLY_CSTREF_PROPERTY( QString, desktopDir )
    QML_READONLY_CSTREF_PROPERTY( QString , fontsDir )
    QML_READONLY_CSTREF_PROPERTY( QString , musicDir )


    // System Info
    QML_READONLY_CSTREF_PROPERTY (QString, buildCpuArchitecture)
    QML_READONLY_CSTREF_PROPERTY ( QString ,currentCpuArchitecture )
    QML_READONLY_CSTREF_PROPERTY ( QString, buildAbi )
    QML_READONLY_CSTREF_PROPERTY ( QString, kernelType )
    QML_READONLY_CSTREF_PROPERTY ( QString, kernelVersion)
    QML_READONLY_CSTREF_PROPERTY ( QString, productType )
    QML_READONLY_CSTREF_PROPERTY ( QString, productVersion)
    QML_READONLY_CSTREF_PROPERTY ( QString, prettyProductName)


//    Screen/Window to come
    QML_READONLY_CSTREF_PROPERTY (qreal, dp)
//    QML_READONLY_CSTREF_PROPERTY ()

public:
    enum FormFactor {
        Phone,
        Phablet,
        Tablet,
        Computer,
        TV,
        Unknown
    };
    Q_ENUM(FormFactor)

    Device(QObject *parent = nullptr);


    FormFactor formFactor() const;
    QString name() const;
    QString iconName() const;

    bool isPortrait() const;
    bool hasTouchScreen() const;
    bool isMobile() const;
    bool hoverEnabled() const;

    int gridUnit() const;
    int dpMultiplier() const;

signals:
    void geometryChanged();

private slots:
    void screenChanged();

private:
    float calculateDiagonal() const;
    QScreen *m_screen;
};

#endif // DEVICE_H
