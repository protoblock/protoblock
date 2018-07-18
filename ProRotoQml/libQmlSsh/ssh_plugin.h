#ifndef SSH_PLUGIN_H
#define SSH_PLUGIN_H

#include <QQmlExtensionPlugin>

class SshPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // SSH_PLUGIN_H
