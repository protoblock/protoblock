#ifndef QQMLSQL_PLUGIN_H
#define QQMLSQL_PLUGIN_H

#include <QQmlExtensionPlugin>

class ProRotoQmlSqlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QQMLSQL_PLUGIN_H
