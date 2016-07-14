/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "device.h"

Device::Device(QObject *parent)
        : QObject(parent)
{
    QGuiApplication *app = (QGuiApplication *) QGuiApplication::instance();
    m_screen = app->primaryScreen();

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    connect(app, &QGuiApplication::primaryScreenChanged,
            this, &Device::screenChanged);
#endif


    QSysInfo systemInfo;
   setbuildCpuArchitecture(systemInfo.buildCpuArchitecture ());
   setcurrentCpuArchitecture(systemInfo.currentCpuArchitecture ());
   setbuildAbi(systemInfo.buildAbi ());
   setkernelType(systemInfo.kernelType ());
   setkernelVersion(systemInfo.kernelVersion ());
   setproductType(systemInfo.productType ());
   setproductVersion(systemInfo.productType ());
   setprettyProductName(systemInfo.prettyProductName ());

    setdp( m_screen->devicePixelRatio () );



    setappDir(QStandardPaths::standardLocations (QStandardPaths::AppDataLocation).first ()) ;
    setappConfigDir (QStandardPaths::standardLocations (QStandardPaths::AppDataLocation).first ());
    setdataDir(QStandardPaths::standardLocations (QStandardPaths::DataLocation).first ()) ;
    setdownloadsDir(QStandardPaths::standardLocations (QStandardPaths::DownloadLocation).first ()) ;
    sethomeDir(QStandardPaths::standardLocations (QStandardPaths::HomeLocation).first ()) ;
    setpicturesDir(QStandardPaths::standardLocations (QStandardPaths::PicturesLocation).first ()) ;
    setdesktopDir(QStandardPaths::standardLocations (QStandardPaths::DesktopLocation).first ()) ;
    setfontsDir(QStandardPaths::standardLocations (QStandardPaths::FontsLocation).first ()) ;
    setmusicDir(QStandardPaths::standardLocations (QStandardPaths::MusicLocation).first ()) ;

}


bool Device::isPortrait() const
{
    return m_screen->physicalSize().height() > m_screen->physicalSize().width();
}

bool Device::hasTouchScreen() const
{
// QTBUG-36007
#if defined(Q_OS_ANDROID)
    return true;
#else
    const auto devices = QTouchDevice::devices();
    foreach(const QTouchDevice *dev, devices) {
        if (dev->type() == QTouchDevice::TouchScreen)
            return true;
    }
    return false;
#endif
}

bool Device::isMobile() const
{
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID) || defined(Q_OS_BLACKBERRY) || defined(Q_OS_QNX) || defined(Q_OS_WINRT)
    return true;
#else
    if (qEnvironmentVariableIsSet("QT_QUICK_CONTROLS_MOBILE")) {
        return true;
    }
    return false;
#endif
}

bool Device::hoverEnabled() const
{
    return !isMobile() || !hasTouchScreen();
}



void Device::screenChanged()
{
    if (m_screen)
        m_screen->disconnect(this);

    QGuiApplication *app = (QGuiApplication *) QGuiApplication::instance();
    m_screen = app->primaryScreen();

    connect(m_screen, &QScreen::geometryChanged, this, &Device::geometryChanged);

    emit geometryChanged();
}

float Device::calculateDiagonal() const
{
    return sqrt(pow((m_screen->physicalSize().width()), 2) +
                pow((m_screen->physicalSize().height()), 2)) * 0.039370;
}
