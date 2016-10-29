#include "plugin.h"
#include "colors.h"
#include "formfactor.h"
#include "palette.h"
#include "units.h"
#include "listener.h"
#include "teamcolors.h"
#include "protoscreen.h"
#include <qqml.h>

void ProRotoQmlThemePlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Theme
    qmlRegisterSingletonType<Colors>(uri, 1, 0, "Colors", colors);
    qmlRegisterSingletonType<FormFactor>(uri, 1, 0, "FormFactor", formFactor);
    qmlRegisterSingletonType<Palette>(uri, 1, 0, "Palette",themes);
    qmlRegisterSingletonType<TeamColors>(uri,1,0,"TeamInfo",teamColors);
    qmlRegisterSingletonType<ProtoScreen>(uri, 1, 0, "ProtoScreen", protoScreen);

//    qmlRegisterUncreatableType<Units>(uri, 1, 0, "Units", "Not instantiable");

}

void ProRotoQmlThemePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri)
    engine->rootContext()->setContextProperty("Unit", &Units::instance());
    ContextPropertyChangeListener *unitsChangeListener =
        new ContextPropertyChangeListener(engine->rootContext(), "Unit");
    QObject::connect(&Units::instance(), SIGNAL(gridUnitChanged()),
                     unitsChangeListener, SLOT(updateContextProperty()));
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

QObject *protoScreen(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    qDebug() << " only once ";
    ProtoScreen *protoScreens = new ProtoScreen();
    return protoScreens;
}
