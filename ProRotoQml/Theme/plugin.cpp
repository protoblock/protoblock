#include "plugin.h"
#include "colors.h"
#include "formfactor.h"
#include "palette.h"
#include "units.h"
#include "listener.h"
#include "teamcolors.h"
#include <qqml.h>

void ProRotoQmlThemePlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Theme
    qmlRegisterSingletonType<Colors>(uri, 1, 0, "Colors", colors);
    qmlRegisterSingletonType<FormFactor>(uri, 1, 0, "FormFactor", formFactor);
    qmlRegisterSingletonType<Palette>(uri, 1, 0, "Palette",themes);
    qmlRegisterSingletonType<TeamColors>(uri,1,0,"TeamInfo",teamColors);

    qmlRegisterSingletonType<Units>(uri, 1, 0, "UnitsSingle",unitsListen);
}

void ProRotoQmlThemePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri)

//    QString applicationDir = QString("%1/").arg(QGuiApplication::applicationDirPath());
//    QString homeDir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
//    QString appDataDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
//    // NOT YET (16.10)
//    //    QString appConfigDir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first();
//    QString downloadsDir = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).first();
//    QString tmpDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first();
//    QString pluginDirs =  QString("%1plugins/").arg(applicationDir);
//    QString userDir = QString("%1Users/").arg(applicationDir);


//    engine->rootContext()->setContextProperty("userDir",userDir);
//    engine->rootContext()->setContextProperty("applicationDir",applicationDir);
//    engine->rootContext()->setContextProperty("homeDir",homeDir);
//    engine->rootContext()->setContextProperty("appDataDir",appDataDir);
//     // NOT YET (16.10)
//     //    engine()->rootContext()->setContextProperty("appConfigDir",appConfigDir);
//    engine->rootContext()->setContextProperty("downloadsDir",downloadsDir);
//    engine->rootContext()->setContextProperty("tmpDir",tmpDir);
//    engine->rootContext()->setContextProperty("pluginDirs", pluginDirs);

////    QQmlContext* context = engine->rootContext();



}

QObject *colors(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Colors *colors = new Colors();
    return colors;
}

QObject *formFactor(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    FormFactor *formFactor = new FormFactor();
    return formFactor;
}

QObject *themes(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Palette *palette = new Palette();
    return palette;
}



QObject *teamColors(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    TeamColors *teamColors = new TeamColors();
    return teamColors;
}

QObject *unitsListen(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    engine->rootContext()->setContextProperty("Unit", Units::instance());
    ContextPropertyChangeListener *unitsChangeListener =
        new ContextPropertyChangeListener(engine->rootContext(), "Unit");

    QObject::connect(Units::instance(), SIGNAL(gridUnitChanged()),
                     unitsChangeListener, SLOT(updateContextProperty()));

    return Units::instance();
}
