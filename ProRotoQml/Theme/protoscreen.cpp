#include "protoscreen.h"
#include <QCoreApplication>
#include <qmath.h>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

ProtoScreen::ProtoScreen(QObject *parent) :
    QObject(parent),
    m_bInitialized(false),
    m_gridUnit(8),
    m_designResolution(QGuiApplication::primaryScreen ()->availableGeometry ()),
    m_scaleSize(1.0),
    m_formFactor("desktop"),
    m_androidDpi(),
    m_windowsDesktopScale(.90),
    m_androidScale(1.0),
    m_tempMacVersion(6.0),
    m_os("windows") {
    initialize();
}


QString ProtoScreen::os() const
{
    return  m_os;
}

void ProtoScreen::initialize() {
    QScreen *desktop = QGuiApplication::primaryScreen();
    double mmToInch = 0.0393700787;

    double displayWidthInch = desktop->size ().width() * mmToInch;
    double displayHeightInch = desktop->size ().height() * mmToInch;
    m_desktopGeometry = desktop->geometry();

    m_displayDiagonalSize = qSqrt(qRound(displayWidthInch*displayWidthInch) + qRound(displayHeightInch*displayHeightInch));

    m_displayDiagonalSize = QString::number(m_displayDiagonalSize,'g',2).toDouble();


    m_devicePixelRatio = desktop->devicePixelRatio();




    //set all the qscreen stuff
    setavailableHeight(desktop->availableGeometry().height() );
    setavailableWidth(desktop->availableGeometry().width() );
    setpixelRatio (desktop->devicePixelRatio ());


    qDebug() << "displayWidthInch" << displayWidthInch;
    qDebug() << "displayHeightInch" << displayHeightInch;
    qDebug() << "m_displayDiagonalSize" << m_displayDiagonalSize;
    qDebug() << "m_devicePixelRatio" << m_devicePixelRatio;
    qDebug() << "desktop->size ().width()" << desktop->size ().width();
    qDebug() << "desktop->size ().height()" << desktop->size ().height();
    qDebug() << "desktop->availableGeometry().height()" << desktop->availableGeometry().height();
    qDebug() << "desktop->availableGeometry().width()" << desktop->availableGeometry().width();
    qDebug() << "desktop->geometry()" << desktop->geometry();

    setavailrect(desktop->availableGeometry());
    updateFormFactor ();
    m_bInitialized = true;



}

void ProtoScreen::setGridUnit(const double &unit) {
    qDebug() << "m_gridUnit" << m_gridUnit;
    qDebug() << "unit" << unit;

    if( unit != 1.0 ) {
        m_gridUnit = (unit * m_gridUnit) ;
        emit gridUnitChanged();
    }

    qDebug() << "m_gridUnit" << m_gridUnit;

    updateFonts();
}

double ProtoScreen::guToPx(double units) {
    return units * m_gridUnit;
}

double ProtoScreen::pxToGu(double px) {
    return px / m_gridUnit;
}


void ProtoScreen::finalFormFactor(const QString &systemType,
                                  const double &versionORscaleSize ,
                                  const double diagonal)
{
    m_systemType = systemType;
    // IOS
    if ( systemType == "ios"){
        if (diagonal >= 3.5 && diagonal < 5) {
            m_formFactor = "phone";
        }
        //iPhone 1st generation to phablet
        else if (diagonal >= 5 && diagonal < 6.5) {
            m_formFactor = "phablet";
        }
        else if (diagonal >= 6.5 && diagonal < 10.1) {
            m_formFactor = "tablet";
        }
        // apple TV
        else if (diagonal > 10.1 ){
            m_formFactor  = "tv";
        }
        setGridUnit(versionORscaleSize);
        setScaleSize(versionORscaleSize);
    }

    // ANDROID
    else if (systemType == "android" ){
        if (diagonal >= 3.5 && diagonal < 5) {
            m_formFactor = "phone";
        }
        //iPhone 1st generation to phablet
        else if (diagonal >= 5 && diagonal < 6.5) {
            m_formFactor = "phablet";
        }
        else if (diagonal >= 6.5 && diagonal < 10.1) {
            m_formFactor = "tablet";
        }
        // android TV
        else if (diagonal > 10.1 ){
            m_formFactor  = "tv";
        }
        setGridUnit(versionORscaleSize);
        setScaleSize(versionORscaleSize);
    }

    // WINDOWS
    else if (
             systemType == "winrt"
             || systemType == "wince"
             || systemType == "windows"
             ){
        if (diagonal >= 3.5 && diagonal < 5) {
            m_formFactor = "phone";
        }
        //iPhone 1st generation to phablet
        else if (diagonal >= 5 && diagonal < 6.5) {
            m_formFactor = "phablet";
        }
        else if (diagonal >= 6.5 && diagonal < 10.1) {
            m_formFactor = "tablet";
        }
        // FIXME TV
        else if (diagonal > 10.1  ){
            m_formFactor  = "desktop";
        }
        setGridUnit(versionORscaleSize);
        setScaleSize(versionORscaleSize);
    }

    //OSX
    if ( systemType == "osx"){
        if (diagonal >= 3.5 && diagonal < 5) {
            m_formFactor = "phone";
        }
        //iPhone 1st generation to phablet
        else if (diagonal >= 5 && diagonal < 6.5) {
            m_formFactor = "phablet";
        }
        else if (diagonal >= 6.5 && diagonal < 10.1) {
            m_formFactor = "tablet";
        }
        // apple TV
        else if (diagonal > 10.1 ){
            m_formFactor  = "desktop";
        }
        setGridUnit(versionORscaleSize);
        setScaleSize(versionORscaleSize);
    }



        qDebug() << "OS " << systemType <<" ScaleSize " << versionORscaleSize << "  Diagonal " << diagonal  << " Device Form factor is " << m_formFactor;

           m_os = systemType;
}



double ProtoScreen::checkIphoneScaleSize(const int &width, const int &height , const double &iPhoneVersion)
{
    if (iPhoneVersion >= 4 ){
        if (width >= 320 && width < 321&& height < 567 ){
            return 1.0;
        }
        else if (height >  567 && height < 569 && width  == 320){
            return 1.0;
        }
        else if (height >  665  && height < 668 && width  == 375)
        {
            return 1.0;
        }
        else if (width >= 374  && height  >= 665 )
        {
            return 1.0;
        }
    }else {
        return 1.0;
    }
    return 1.0;
}


void ProtoScreen::updateFormFactor(){

    QScreen *m_screen = QGuiApplication::primaryScreen ();
    QSysInfo sysInfo;


    double m_169 =  qSqrt (pow((m_screen->physicalSize().width()), 2) +
                           qPow((m_screen->physicalSize().height()), 2)) * 0.039370;

    qDebug() << " jayberg " << m_169 << m_displayDiagonalSize;

    qDebug() << "m_169" << m_169;
    qDebug() << "m_screen->physicalSize().width())" << m_screen->physicalSize().width();
    qDebug() << "m_screen->physicalSize().height()" << m_screen->physicalSize().height();
    qDebug() << " m_screen->logicalDotsPerInch() " << m_screen->logicalDotsPerInch();


//    m_169 = m_displayDiagonalSize;

    // Check to see if this is a ios
    if ( sysInfo.productType () == "ios") {
        // NOw that we know it is a IOS
        // lets look at version of mac it is
        if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_4_3){
            m_tempMacVersion = 4.3;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_5_0){
            m_tempMacVersion =  5.0;
        }
        else if (sysInfo.MacintoshVersion ==  QSysInfo::MV_IOS_5_1){
            m_tempMacVersion =  5.1;
        }
        else if (sysInfo.MacintoshVersion ==   QSysInfo::MV_IOS_6_0){
            m_tempMacVersion =  6.0;
        }
        else if (sysInfo.MacintoshVersion ==  QSysInfo::MV_IOS_6_1){
            m_tempMacVersion =  6.1;
        }
        else if (sysInfo.MacintoshVersion ==  QSysInfo::MV_IOS_7_0){
            m_tempMacVersion =  7.0;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_7_1){
            m_tempMacVersion =   7.1;
        }
        else if (sysInfo.MacintoshVersion ==  QSysInfo::MV_IOS_8_0){
            m_tempMacVersion =  8.0;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_8_1){
            m_tempMacVersion =   8.1;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_8_2){
            m_tempMacVersion = 8.2;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_8_3){
            m_tempMacVersion =  8.3;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_8_4){
            m_tempMacVersion =  8.4;
        }
        else if (sysInfo.MacintoshVersion == QSysInfo::MV_IOS_9_0){
            m_tempMacVersion =  9.0;
        }
        finalFormFactor ("ios" ,
                         checkIphoneScaleSize(
                             m_screen->size ().width (),
                             m_screen->size ().height (),
                             m_tempMacVersion
                             ),
                         m_169
                         );
        //        delete m_screen;
        return;
    }
    // END IOS


    // START ANDROID / LINUX
    else  if (sysInfo.productType () == "android")
    {
        // Check the BuildArch to see if arm or arm 64 .  Also look at x86_64 android
        if ( sysInfo.buildCpuArchitecture() == "arm" || sysInfo.buildCpuArchitecture() == "arm64")
        {
            // SOURCE:
            //            https://developer.android.com/guide/practices/screens_support.html
            //(low) 120dpi

            if(m_screen->logicalDotsPerInch () <= 120)
            {
                m_androidDpi = "ldpi";
                m_androidScale = 1.0;
            }

            else if (m_screen->logicalDotsPerInch() <= 160)
            {
                m_androidDpi = "mdpi";
                m_androidScale = 1.5;
            }
            //(high) ~240dpi
            else if (m_screen->logicalDotsPerInch() <= 240)
            {
                m_androidDpi = "hdpi";
                m_androidScale = 2.0;
            }
            //(high) ~240dpi
            else if (m_screen->logicalDotsPerInch()  <= 320)
            {
                m_androidDpi = "xhdpi" ;
                m_androidScale = 3.0;
            }
            // (extra-high) ~320dpi
            else if (m_screen->logicalDotsPerInch() <= 480)
            {
                m_androidDpi = "xxhdpi" ;
                m_androidScale  = 4.0;
            }

            // (extra-extra-high) ~480dpi
            else if (m_screen->logicalDotsPerInch() <= 640 )
            {
                m_androidDpi = "xxxhdpi";
                m_androidScale = 5.0;
            }

            //(extra-extra-extra-high) ~640dpi
            else //if (m_screen->logicalDotsPerInch() >= 640)
            {
                qDebug() << "m_screen->logicalDotsPerInch() > 640";

                m_androidDpi = "xxxhdpi";
                m_androidScale = 5.0;
            }
        }
        else
        {
            qDebug() << "we know that it is android but we do not know the DPI so we have to make another work around";
            //            delete m_screen;
            return;
        }

//        m_androidScale *= 1.10;
        qDebug() << "android  Scale " <<  m_androidScale << "  diag " << m_169 << "m_screen->logicalDotsPerInch()" << m_screen->logicalDotsPerInch();
        finalFormFactor ("android" , m_androidScale, m_169);
        //        delete m_screen;
        return;
    } // END ANDROID



    // WINDOWS PHONE
    else if ( sysInfo.productType() == "winphone"){
        //        FIXME
    }
    // WINDOWS LOOK FOR DPI
    else if ( sysInfo.productType () == "winrt"
             || sysInfo.productType () == "wince"
             || sysInfo.productType () == "windows"
             )
    {
        double newwindowsscale = 1.0;


        //        SOURCE
        //      https://msdn.microsoft.com/en-us/library/windows/desktop/dn469266(v=vs.85).aspx
        if (m_169 <= 10.5){
            qDebug() << "This shit is small !";
        }
        else if (m_169 <=  11.5){
            if (m_screen->size().width() >= 1920 && m_screen->size().height() >= 1080){
                newwindowsscale = 1.5;
            }
        }
        else if (m_169 <= 13.2){
            if (m_screen->size().width() >= 1920 && m_screen->size().height() >= 1200){
                newwindowsscale = 1.5;
            }
        }
        else if (m_169  <= 15.3){
            if(m_screen->logicalDotsPerInch() >= 192)
                newwindowsscale = 2.0;
            else if ( m_screen->logicalDotsPerInch() >= 144) {
                newwindowsscale = 1.5;
            }
        }
        else if (m_169 <= 16.9){
            if ( m_screen->logicalDotsPerInch() >= 120 && m_screen->logicalDotsPerInch()  < 192){
                newwindowsscale = 1.25;
            }
            else if (m_screen->logicalDotsPerInch() >= 192  )
            {
                newwindowsscale = 2.0;
            }
        }
        else if (m_169 < 23){
            if ( m_screen->logicalDotsPerInch() >= 120 && m_screen->logicalDotsPerInch()  < 192){
                newwindowsscale = 1.25;
            }
            if (m_screen->logicalDotsPerInch() >= 192){
                newwindowsscale = 2.0;
            }
        }
        else if (m_169 >=  23 ){
            if ( m_screen->logicalDotsPerInch() >= 120 && m_screen->logicalDotsPerInch()  < 192){
                newwindowsscale = 1.25;
            }
            else if (m_screen->logicalDotsPerInch() >= 192  )
            {
                newwindowsscale = 2.0;
            }
        }
        else {
            newwindowsscale = 1.0;
        }

        m_windowsDesktopScale *= newwindowsscale;
        finalFormFactor ("windows", m_windowsDesktopScale,m_169);
        return;
    }
    // END WINDOWS

    // MACOSX

    if(sysInfo.productType() == "osx"){
        finalFormFactor ("osx", m_windowsDesktopScale , m_169);
        return;
    }
    // START LINUX (SOMETIMES ANDROID COes back as Linux)
    //        if ( sysInfo.buildCpuArchitecture() === "arm" || systemInfo.buildCpuArchitecture == "arm64" && systemInfo.productType() == "android" || sysInfo.productType () == "linux")
    //        {
    //            // Ok know that we know that we are on a armv7 lets look deeper
    //        }

}

double ProtoScreen::scaleSize() const
{
    return m_scaleSize;
}

void ProtoScreen::setScaleSize(const double &size)
{
    qDebug() << "m_scaleSize" << m_scaleSize;
    qDebug() << "size" << size;


    if ( m_scaleSize == size)
        return;
    m_scaleSize = size;
    emit scaleSizeChanged();
}

void ProtoScreen::updateFonts() {

    if (m_formFactor == "desktop") {
        m_fonts[XXLARGE] = guToPx(5);
        m_fonts[XLARGE] = guToPx(4.7);
        m_fonts[LARGE] = guToPx(4);
        m_fonts[MEDIUM] = guToPx(3.5);
        m_fonts[NORMAL] = guToPx(2.5);
        m_fonts[SMALL] = guToPx(2);
        m_fonts[VERYSMALL] = guToPx(1.6);
        m_fonts[TINY] = guToPx(1.2);

        m_fonts[XXLARGE] = guToPx(5);
        m_fonts[XXLARGE] = guToPx(4.7);
        m_fonts[XLARGE] = guToPx(4);
        m_fonts[LARGE] = guToPx(3.5);
        m_fonts[MEDIUM] = guToPx(2.5);
        m_fonts[NORMAL] = guToPx(2);
        m_fonts[SMALL] = guToPx(1.6);
        m_fonts[VERYSMALL] = guToPx(1.2);
        m_fonts[TINY] = guToPx(.90);
    }

    else if (m_formFactor == "tv") {
        m_fonts[XXLARGE] = guToPx(10);
        m_fonts[XLARGE] = guToPx(8);
        m_fonts[LARGE] = guToPx(6);
        m_fonts[MEDIUM] = guToPx(4.5);
        m_fonts[NORMAL] = guToPx(3.5);
        m_fonts[SMALL] = guToPx(3);
        m_fonts[VERYSMALL] = guToPx(2.5);
        m_fonts[TINY] = guToPx(2);
    }
    // FIXME make this with android and iphone options
    else if (m_formFactor == "tablet") {
        m_fonts[XXLARGE] = guToPx(5);
        m_fonts[XLARGE] = guToPx(4.7);
        m_fonts[LARGE] = guToPx(4);
        m_fonts[MEDIUM] = guToPx(3.5);
        m_fonts[NORMAL] = guToPx(2.5);
        m_fonts[SMALL] = guToPx(2);
        m_fonts[VERYSMALL] = guToPx(1.5);
        m_fonts[TINY] = guToPx(1.2);
    }

    // FIXME make this with android and iphone options
    else if (m_formFactor == "phone" && m_systemType == "ios"){
        m_fonts[XXLARGE] = guToPx(5);
        m_fonts[XLARGE] = guToPx(4.6);
        m_fonts[LARGE] = guToPx(3.0);
        m_fonts[MEDIUM] = guToPx(2.8);
        m_fonts[NORMAL] = guToPx(2.0);
        m_fonts[SMALL] = guToPx(1.5);
        m_fonts[VERYSMALL] = guToPx(1.0);
        m_fonts[TINY] = guToPx(.5);
    }
    else if (m_formFactor == "phone" && m_systemType == "android"){
        m_fonts[XXLARGE] = guToPx(6);
        m_fonts[XLARGE] = guToPx(5.6);
        m_fonts[LARGE] = guToPx(4.3);
        m_fonts[MEDIUM] = guToPx(3.8);
        m_fonts[NORMAL] = guToPx(3.0);
        m_fonts[SMALL] = guToPx(2.5);
        m_fonts[VERYSMALL] = guToPx(2.0);
        m_fonts[TINY] = guToPx(1.2);
    }
}
