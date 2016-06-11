
#include "protoscreen.h"
#include <QCoreApplication>
#include <QDesktopWidget>
#include <qmath.h>
#include <QScreen>
#include <QGuiApplication>

ProtoScreen* ProtoScreen::m_pInstance = NULL;

ProtoScreen::ProtoScreen(QObject *parent) :
    QObject(parent), m_bInitialized(false)
{
     //m_designResolution = QRect(0,0,1366,768);


     initialize();
}

QObject* ProtoScreen::singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    return ProtoScreen::instance(engine);
}

void ProtoScreen::initialize() {
//    QDesktopWidget* desktop = qApp->desktop();
    QScreen * desktop = QGuiApplication::primaryScreen();
    m_designResolution = desktop->availableGeometry();
    QSizeF displaySize = qApp->primaryScreen()->physicalSize();
    double mmToInch = 0.0393700787;
    double displayWidthInch = displaySize.width()*mmToInch;
    double displayHeightInch = displaySize.height()*mmToInch;

    m_desktopGeometry = desktop->geometry();
    m_displayDiagonalSize = qSqrt(qRound(displayWidthInch*displayWidthInch) + qRound(displayHeightInch*displayHeightInch));
    m_displayDiagonalSize = QString::number(m_displayDiagonalSize,'g',2).toDouble();

    m_defaultGrid = 6;

#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
    m_devicePixelRatio = ((double)m_desktopGeometry.width()) / ((double)m_designResolution.width());
#else
    m_devicePixelRatio = qApp->devicePixelRatio();
#endif

    updateGridUnit();
    m_bInitialized = true;
}

void ProtoScreen::updateGridUnit() {
#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
    // On desktop we need to calculate using the pure resolution
    setGridUnit(m_defaultGrid * m_devicePixelRatio);
#else
// On mobile devices we need to take care to check the size of the display as well.
#if defined(Q_OS_IOS)
    if (ProtoDevice::deviceModelIdentifier().contains("iPad"))
            setGridUnit(m_desktopGeometry.width()/133);
    else setGridUnit(m_desktopGeometry.width()/80);
#elif defined(Q_OS_ANDROID)
    if (ProtoDevice::deviceModelIdentifier().contains("AndroidTablet"))
       setGridUnit(m_desktopGeometry.width()/128);
    else
        setGridUnit(m_desktopGeometry.width()/80);
#endif
#endif
}

void ProtoScreen::setGridUnit(double unit) {
    m_gridUnit = unit;
    if (m_gridUnit<=0) {
        m_gridUnit = m_defaultGrid;
    }

    updateFonts();
    emit gridUnitChanged();
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

void ProtoScreen::setDesignResolution(int width, int height) {
    m_designResolution = QRect(0,0,width,height);
    initialize();
}

void ProtoScreen::updateFonts() {
    m_fonts[FONT_XXLARGE] = guToPx(5);
    m_fonts[FONT_XLARGE] = guToPx(4.7);
    m_fonts[FONT_LARGE] = guToPx(4);
    m_fonts[FONT_MEDIUM] = guToPx(3.5);
    m_fonts[FONT_NORMAL] = guToPx(2.5);
    m_fonts[FONT_SMALL] = guToPx(2);
    m_fonts[FONT_TINY] = guToPx(1.2);
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

