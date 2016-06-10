#ifndef TORRENTSERIALIZER_H
#define TORRENTSERIALIZER_H

#include <QObject>

class TorrentModel;
class QSettings;
class QTimer;

class TorrentSerializer : public QObject
{
    Q_OBJECT
public:
    explicit TorrentSerializer( TorrentModel *model, QObject *parent = 0);
    ~TorrentSerializer();

    TorrentModel *model() const;

    /// Shoud be called after TorrentSerializer::load()
    void startAutoSave();

    int saveTimeSec() const;
    void setSaveTimeSec(int saveTimeSec);

public slots:
    void save();
    void load();

private slots:
    void onModelDestroyed();

private:
    TorrentModel            *m_model;
    QSettings               *m_settings;
    QTimer                  *m_saveTimer;
    int                      m_saveTimeSec;

    void                     initSaveTimer();

static int SaveTimeSecDefault;
};

#endif // TORRENTSERIALIZER_H
