#ifndef AUDIO_PLUGIN_H
#define AUDIO_PLUGIN_H

#include <QQmlExtensionPlugin>

class ProRotoQmlAudioPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // AUDIO_PLUGIN_H
