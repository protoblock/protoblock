
#include "protoscreen.h"
#include <QCoreApplication>
#include <qmath.h>
#include <QScreen>
#include <QGuiApplication>



// ANDROID ONLY FIXME
//#define QT_OS_ANDROID
//    #include <QPlatformNativeInterface>
//    #include <jni.h>
//#endif


ProtoScreen* ProtoScreen::m_pInstance = NULL;


ProtoScreen::ProtoScreen(QObject *parent) :
    QObject(parent),
    m_bInitialized(false),
    m_gridUnit(6),
    m_designResolution(QGuiApplication::primaryScreen ()->availableGeometry ()),
    m_scaleSize(1.0),
    m_formFactor("desktop")
{
    initialize();
}

QObject* ProtoScreen::singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    return ProtoScreen::instance(engine);
}

void ProtoScreen::initialize() {

    QScreen *desktop = QGuiApplication::primaryScreen();

    //    QSizeF displaySize =  desktop->physicalSize ();
    // convert the mm to the inch's helper
    double mmToInch = 0.0393700787;

    double displayWidthInch = desktop->size ().width() * mmToInch;
    double displayHeightInch = desktop->size ().height() * mmToInch;
    m_desktopGeometry = desktop->geometry();

    m_displayDiagonalSize = qSqrt(qRound(displayWidthInch*displayWidthInch) + qRound(displayHeightInch*displayHeightInch));

    m_displayDiagonalSize = QString::number(m_displayDiagonalSize,'g',2).toDouble();
    // maybe this should be 72
    //    m_defaultGrid = 6;

    //#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
    //    m_devicePixelRatio = ( (double)m_desktopGeometry.width() ) / ((double)m_designResolution.width());
    //#else
    m_devicePixelRatio = desktop->devicePixelRatio();
    //#endif

    updateFormFactor ();
    m_bInitialized = true;
    // delete desktop;

}



// FIXME THIS IS THE CAUSE OF ALL OF THIS !
//void ProtoScreen::updateGridUnit() {
//#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
//    // On desktop we need to calculate using the pure resolution
//    setGridUnit(m_defaultGrid * m_devicePixelRatio);
//#else
// On mobile devices we need to take care to check the size of the display as well.
//#if defined(Q_OS_IOS)
//    if (ProtoDevice::deviceModelIdentifier().contains("iPad"))
//            setGridUnit(m_desktopGeometry.width()/133);
//    else setGridUnit(m_desktopGeometry.width()/80);
//#elif defined(Q_OS_ANDROID)
//    if (ProtoDevice::deviceModelIdentifier().contains("AndroidTablet"))
//       setGridUnit(m_desktopGeometry.width()/128);
//    else
//        setGridUnit(m_desktopGeometry.width()/80);
//#endif
//#endif
//}


//LEAK on signal
void ProtoScreen::setGridUnit(double unit) {
    if( m_gridUnit == unit ){
        return;
    }else {
        m_gridUnit = unit;
        emit gridUnitChanged();
    }
     updateFonts();
}


ProtoScreen* ProtoScreen::instance(QQmlEngine *engine) {
    if (!m_pInstance) {
        m_pInstance = new ProtoScreen(engine);
    }
    return m_pInstance;
}

double ProtoScreen::guToPx(double units) {
    return units * m_gridUnit;
}

double ProtoScreen::pxToGu(double px) {
    return px / m_gridUnit;
}

//void ProtoScreen::setDesignResolution(int width, int height) {
//    m_designResolution = QRect(0,0,width,height);
//    initialize();
//}





void ProtoScreen::finalFormFactor(const QString &systemType, const double &versionORscaleSize , const double diagonal)
{

    qDebug() << "LOOK !!!!! " << systemType <<" " << versionORscaleSize << "  " << diagonal ;


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
        // FIXME
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





}



double ProtoScreen::checkIphoneScaleSize(const int &width, const int &height , const double &iPhoneVersion)
{
    if (iPhoneVersion >= 4 ){
        if (width >= 320 && width < 321&& height < 567 ){
            return 2.0;
        }
        else if (height >  567 && height < 569 && width  == 320){
            return 2.0;
        }
        else if (height >  665  && height < 668 && width  == 375)
        {
            return 2.0;
        }
        else if (width >= 374  && height  >= 665 )
        {
            return 3.0;
        }
    }else {
        return 1.0;
    }
    return 1.0;
}


void ProtoScreen::updateFormFactor(){

    QScreen *m_screen = QGuiApplication::primaryScreen ();
    QSysInfo sysInfo;
    QString m_androidDpi;
    qDebug() <<"fjksldhafdhsahfkdshskvah " <<  sysInfo.productType ();
    double m_windowsDesktopScale;
    double m_androidScale;
    double m_tempMacVersion;
    //    float m_169 = qSqrt(qRound(displayWidthInch*displayWidthInch) + qRound(displayHeightInch*displayHeightInch));

    double m_169 =  qSqrt (pow((m_screen->physicalSize().width()), 2) +
                           qPow((m_screen->physicalSize().height()), 2)) * 0.039370;

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
            if (m_screen->logicalDotsPerInch() >= 120
                    && m_screen->logicalDotsPerInch() < 160)
            {
                m_androidDpi = "ldpi";
                m_androidScale = 0.75;
            }
            //(high) ~240dpi
            else if (m_screen->logicalDotsPerInch() >= 160
                     && m_screen->logicalDotsPerInch() < 240)
            {
                m_androidDpi = "mdpi";
                m_androidScale = 1.0;
            }
            //(high) ~240dpi
            else if (m_screen->logicalDotsPerInch() >= 240
                     && m_screen->logicalDotsPerInch()  < 319)
            {
                m_androidDpi = "hdpi" ;
                m_androidScale = 1.5;
            }
            // (extra-high) ~320dpi
            else if (m_screen->logicalDotsPerInch() >= 320
                     && m_screen->logicalDotsPerInch() < 479)
            {
                m_androidDpi = "xhdpi" ;
                m_androidScale  = 2.0;
            }

            // (extra-extra-high) ~480dpi
            else if (m_screen->logicalDotsPerInch() >= 480
                     && m_screen->logicalDotsPerInch() < 639 )
            {
                m_androidDpi = "xxhdpi";
                m_androidScale = 3.0;
            }

            //(extra-extra-extra-high) ~640dpi
            else if (m_screen->logicalDotsPerInch() >= 640)
            {
                m_androidDpi = "xxxhdpi";
                m_androidScale = 4.0;
            }
        }
        else
        {
            qDebug() << "we know that it is android but we do not know the DPI so we have to make another work around";
            //            delete m_screen;
            return;
        }
        finalFormFactor ("android" , m_androidScale, m_169);
        //        delete m_screen;
        return;
    } // END ANDROID



    // WINDOWS PHONE
    else if ( sysInfo.productType() == "winphone"){
        //        FIXME
    }
    // WINDOWS LOOK FOR DPI
    else if (
             sysInfo.productType () == "winrt"
             || sysInfo.productType () == "wince"
             || sysInfo.productType () == "windows"
             )
    {
        //        SOURCE
        //      https://msdn.microsoft.com/en-us/library/windows/desktop/dn469266(v=vs.85).aspx
        if (m_169 <= 10.5){
            qDebug() << "This shit is small !";
        }
        else if (m_169 >=  10.6 && m_169 <=  11.5){
            if (m_screen->size().width() >= 1920 && m_screen->size().height() >= 1080){
                m_windowsDesktopScale = 1.5;
            }
        }
        else if (m_169 >=  11.6 && m_169 <= 13.2){
            if (m_screen->size().width() >= 1920 && m_screen->size().height() >= 1200){
                m_windowsDesktopScale = 1.5;
            }
        }
        else if (m_169 >=  13.3 && m_169 <= 15.3){
            if(m_screen->logicalDotsPerInch() >= 192 && m_screen->logicalDotsPerInch() >145) {
                m_windowsDesktopScale = 2;
            }
        }
        else if (m_169 >=  15.4 && m_169 <= 16.9){
            if ( m_screen->logicalDotsPerInch() >= 120 && m_screen->logicalDotsPerInch()  < 192){
                m_windowsDesktopScale = 1.25;
            }
            else if (m_screen->logicalDotsPerInch() >= 192  )
            {
                m_windowsDesktopScale = 2;
            }
        }
        else if (m_169 >=  23 && m_169 < 24){
            if (m_screen->logicalDotsPerInch() >= 192){
                m_windowsDesktopScale = 2;
            }
        }
        else if (m_169 >=  23 && m_169 < 24){
            if (m_screen->logicalDotsPerInch() == 120){
                m_windowsDesktopScale = 1.25;
            }
        }
        else {
            finalFormFactor ("windows" , 1,m_169);
            //        delete m_screen;
            return;
        }
        finalFormFactor ("windows", m_windowsDesktopScale,m_169);
        //        delete m_screen;
        return;
    }
    // END WINDOWS

    // MACOSX

    if(sysInfo.productType () == "osx"){
        finalFormFactor ("osx", 1.0 , m_169);
        //        delete m_screen;
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
    if ( m_scaleSize == size)
        return;
    m_scaleSize = size;
    emit scaleSizeChanged();
}


void ProtoScreen::updateFonts() {

    if (m_formFactor == "desktop") {
        m_fonts[FONT_XXLARGE] =guToPx(30);
        m_fonts[FONT_XLARGE] = guToPx(20);
        m_fonts[FONT_LARGE] = guToPx(17);
        m_fonts[FONT_MEDIUM] = guToPx(15);
        m_fonts[FONT_NORMAL] = guToPx(15);
        m_fonts[FONT_SMALL] = guToPx(12);
        m_fonts[FONT_TINY] = guToPx(10);
    }

    else if (m_formFactor == "tv") {
        m_fonts[FONT_XXLARGE] =guToPx(90);
        m_fonts[FONT_XLARGE] = guToPx(60);
        m_fonts[FONT_LARGE] = guToPx(40);
        m_fonts[FONT_MEDIUM] = guToPx(35);
        m_fonts[FONT_NORMAL] = guToPx(35);
        m_fonts[FONT_SMALL] = guToPx(30);
        m_fonts[FONT_TINY] = guToPx(12);
    }

    else if (m_formFactor == "tablet") {
        m_fonts[FONT_XXLARGE] =guToPx(27);
        m_fonts[FONT_XLARGE] = guToPx(20);
        m_fonts[FONT_LARGE] = guToPx(17);
        m_fonts[FONT_MEDIUM] = guToPx(15);
        m_fonts[FONT_NORMAL] = guToPx(15);
        m_fonts[FONT_SMALL] = guToPx(12);
        m_fonts[FONT_TINY] = guToPx(10);
    }
    else if (m_formFactor == "phone"){
        m_fonts[FONT_XXLARGE] =guToPx(27);
        m_fonts[FONT_XLARGE] = guToPx(20);
        m_fonts[FONT_LARGE] = guToPx(17);
        m_fonts[FONT_MEDIUM] = guToPx(15);
        m_fonts[FONT_NORMAL] = guToPx(15);
        m_fonts[FONT_SMALL] = guToPx(12);
        m_fonts[FONT_TINY] = guToPx(10);
    }
}




















#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#endif

#ifdef Q_OS_IOS
#include <sys/sysctl.h>
#endif
/*!
           * \qmltype ProtoDevice
           * \inqmlmodule Mango 0.1
           * \ingroup Mango
           * \inherits Item
           * \since 5.3
           *  The ProtoDevice Qml Plugin. A simple plugin that can retuen the id of a device.
           */

/*!
          \qmlproperty string ProtoDevice::deviceModelIdentifier
          returns back the model id also see deviceModelIdentifier()
          */


/*!
            \qmlsignal ProtoDevice::deviceModelIdentifier()
            returns the device model id
          */
ProtoDevice* ProtoDevice::m_pInstance = NULL;
QString ProtoDevice::m_deviceModelIdentifier = "";

ProtoDevice* ProtoDevice::instance(QQmlEngine *engine) {
    if (!m_pInstance) {
        m_pInstance = new ProtoDevice(engine);
    }
    return m_pInstance;
}

ProtoDevice::ProtoDevice(QObject *parent) :
    QObject(parent)
{
#if defined(Q_OS_IOS)
    static const char key[] = "hw.machine";

    size_t size;
    sysctlbyname(key, NULL, &size, NULL, 0);

    char value[size];
    sysctlbyname(key, &value, &size, NULL, 0);

    m_deviceModelIdentifier = QString::fromLatin1(value);
#elif defined (Q_OS_ANDROID)
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resource = activity.callObjectMethod("getResources","()Landroid/content/res/Resources;");
    QAndroidJniObject configuration = resource.callObjectMethod("getConfiguration","()Landroid/content/res/Configuration;");

    int screenLayout = configuration.getField<int>("screenLayout");
    int SCREENLAYOUT_SIZE_MASK = 0x0000000f;
    int SCREENLAYOUT_SIZE_LARGE = 0x00000003;
    bool isLargeScreen = (screenLayout & SCREENLAYOUT_SIZE_MASK) >= SCREENLAYOUT_SIZE_LARGE;
    if (isLargeScreen) m_deviceModelIdentifier = "AndroidTablet";
    else m_deviceModelIdentifier = "AndroidPhone";
#else
    m_deviceModelIdentifier = "desktop";
#endif
    emit dataChanged();
}

QObject* ProtoDevice::singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    return ProtoDevice::instance(engine);
}
