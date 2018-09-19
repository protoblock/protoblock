#include "plugin.h"
#include <qqml.h>

static QObject *colors(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Colors *colors = new Colors();
    return colors;
}

static QObject *formFactor(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    FormFactor *formFactor = new FormFactor();
    return formFactor;
}

static QObject *themes(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Palette *palette = new Palette();
    return palette;
}

static QObject *teamColors(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    TeamColors *teamColors = new TeamColors();
    return teamColors;
}

static QObject *protoScreen(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    ProtoScreen *protoScreens = new ProtoScreen();
    return protoScreens;
}

void ProtoblockQmlThemePlugin::registerTypes(const char *uri)
{
    // @uri ProtoblockQml.Theme
    qmlRegisterSingletonType<Colors>(uri, 1, 0, "Colors", &colors);
    qmlRegisterSingletonType<FormFactor>(uri, 1, 0, "FormFactor", &formFactor);
    qmlRegisterSingletonType<Palette>(uri, 1, 0, "Palette", &themes);
    qmlRegisterSingletonType<TeamColors>(uri,1,0,"TeamInfo", &teamColors);
    qmlRegisterSingletonType<ProtoScreen>(uri, 1, 0, "ProtoScreen", &protoScreen);
}

void ProtoblockQmlThemePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri)
    engine->rootContext()->setContextProperty("Unit", &Units::instance());
    ContextPropertyChangeListener *unitsChangeListener =
        new ContextPropertyChangeListener(engine->rootContext(), "Unit");
    QObject::connect(&Units::instance(), SIGNAL(gridUnitChanged()),
                     unitsChangeListener, SLOT(updateContextProperty()));
}
