#include "audio_plugin.h"
#include "src/audio.h"
#include <qqml.h>

void ProRotoQmlAudioPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Audio
    qmlRegisterType<Audio>(uri, 1, 0, "AudioRecorder");
}

