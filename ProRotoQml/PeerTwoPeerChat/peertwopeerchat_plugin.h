#ifndef PEERTWOPEERCHAT_PLUGIN_H
#define PEERTWOPEERCHAT_PLUGIN_H

#include <QQmlExtensionPlugin>

class PeerTwoPeerChatPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // PEERTWOPEERCHAT_PLUGIN_H
