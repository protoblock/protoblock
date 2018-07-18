#include "audio.h"

Audio::Audio(QObject *parent):
    QObject(parent),
    m_codec("audio/PCM"),
    m_input("default:"),
    m_quality(4),
    m_volume(100),
    m_state(QMediaRecorder::StoppedState),
    m_error(QMediaRecorder::ResourceError)
{
    audioRecorder = new QAudioRecorder;

    connect(audioRecorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(_q_durationChanged()));
    connect(audioRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this,
            SLOT(_q_stateChanged()));
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(_q_error()));
}

void Audio::_q_error()
{
   m_error = audioRecorder->error();
   m_errorString = audioRecorder->errorString();

   emit errorChanged();
}

Audio::Error Audio::error() const
{
    return Error(m_error);
}

QString Audio::errorString() const
{
    return m_errorString;
}

void Audio::_q_stateChanged()
{
    const QMediaRecorder::State oldState = m_state;
    const QMediaRecorder::State state = audioRecorder->state();

    m_state = audioRecorder->state();

    if (state != oldState) {
        switch (state) {
            case QMediaRecorder::StoppedState:
                emit stopped();
                break;
            case QMediaRecorder::RecordingState:
                if (oldState == QMediaRecorder::PausedState) {
                    emit resumed();
                    break;
                } else {
                    emit recording();
                    break;
                }
            case QMediaRecorder::PausedState:
                emit paused();
                break;
            }

        emit stateChanged();
    }
}

Audio::State Audio::state() const
{
    return State(m_state);
}

QString Audio::path() const
{
    return m_path;
}

void Audio::setPath(const QString &path)
{
    if (m_path == path)
        return;

    m_path = path;
    emit pathChanged();
}

QString Audio::codec() const
{
    return m_codec;
}

void Audio::setCodec(const QString &codec)
{
    if (m_codec == codec)
        return;

    m_codec = codec;
    emit codecChanged();
}

QString Audio::input() const
{
    return m_input;
}

void Audio::setInput(const QString &input)
{
    if (m_input == input)
        return;

    m_input = input;
    emit inputChanged();
}

int Audio::quality() const
{
    return m_quality;
}

void Audio::setQuality(const int &quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    emit qualityChanged();
}

qreal Audio::volume() const
{
    return m_volume;
}

void Audio::setVolume(const qreal &volume)
{
    if (m_volume == volume)
        return;

    m_volume = volume;
    emit volumeChanged();
}

qint64 Audio::duration() const
{
    return m_duration;
}

void Audio::_q_durationChanged()
{
    m_duration = audioRecorder->duration();
    emit durationChanged();
}

void Audio::start()
{
    if (audioRecorder->state() == QMediaRecorder::StoppedState) {
        QAudioEncoderSettings audioSettings;

        // Set codec
        audioSettings.setCodec(m_codec);

        // Set quality: 0=VeryLow, 1=Low, 2=Normal, 3=High, 4=VeryHigh
        audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
        audioSettings.setQuality(QMultimedia::EncodingQuality(m_quality));

        if (audioSettings.codec() == "audio/PCM" || "audio/FLAC") {
            audioSettings.setSampleRate(-1);

            if (m_quality == 0)
                {audioSettings.setSampleRate(8000);}

            if (m_quality == 1)
                {audioSettings.setSampleRate(16000);}

            if (m_quality == 2)
                {audioSettings.setSampleRate(22050);}

            if (m_quality == 3)
                {audioSettings.setSampleRate(44100);}

            if (m_quality == 4)
                {audioSettings.setSampleRate(88200);}
            }

        audioRecorder->setEncodingSettings(audioSettings);
        audioRecorder->setAudioInput(m_input);
        audioRecorder->setContainerFormat(getContainerFromCodec(m_codec));

        // Set volume
        audioRecorder->setVolume(m_volume);

        // Set output location
        cPath = m_path + getExtensionFromCodec(m_codec);
        audioRecorder->setOutputLocation(QUrl(cPath));

        audioRecorder->record();
    }
}

QString Audio::getFilePath()
{
    return cPath;
}

void Audio::stop()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState ||
            audioRecorder->state() == QMediaRecorder::PausedState) {

        audioRecorder->stop();
    }
}

void  Audio::pause()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState) {
        audioRecorder->pause();
    }
}

void Audio::resume()
{
    if (audioRecorder->state() == QMediaRecorder::PausedState) {
        audioRecorder->record();
    }
}

// It parses codec name to get the corrisponding container
QString Audio::getContainerFromCodec(QString codec)
{
    QString container = "";

    if (codec == "audio/vorbis")
        { container = "ogg";}

    if (codec == "audio/PCM")
        { container = "wav";}

    if (codec == "audio/FLAC")
        { container = "raw";}

    if (codec == "audio/AMR")
        { container = "amr";}

    if (codec == "audio/mpeg")
        { container = "raw";}

    return container;
}

// It parses codec name to get the corrisponding extension
QString Audio::getExtensionFromCodec(QString codec)
{
    QString extension = "";

    if (codec == "audio/vorbis")
        { extension = ".ogg";}

    if (codec == "audio/PCM")
        { extension = ".wav";}

    if (codec == "audio/FLAC")
        { extension = ".flac";}

    if (codec == "audio/AMR")
        { extension = ".amr";}

    if (codec == "audio/mpeg")
        { extension = ".mp3";}

    return extension;
}

QStringList Audio::getSupportedCodecs()
{
    QStringList allSupportedCodecs = audioRecorder->supportedAudioCodecs();
    QStringList codecsList;

    foreach (QString codec, allSupportedCodecs) {
        if (codec.contains("audio/mpeg", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/vorbis", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/FLAC", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/PCM", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/AMR", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
    }

    return codecsList;
}

QStringList Audio::getAudioInputs()
{
    return audioRecorder->audioInputs();
}

Audio::~Audio()
{
    delete audioRecorder;
}
