#ifndef UTILS_PLUGIN_H
#define UTILS_PLUGIN_H

#include <QQmlExtensionPlugin>

class ProRotoQmlUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};


//#include "utils_plugin.moc"
#endif // UTILS_PLUGIN_H
