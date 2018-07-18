#ifndef PLUGININISETTER_H
#define PLUGININISETTER_H

#include <QObject>
#include <QString>
#include <QSettings>

class PluginIniSetter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)

public:
    explicit PluginIniSetter(QObject *parent = 0);

    QString type() const;
    void setType(const QString &type);

    QString name() const;
    void setName(const QString &name);

    QString version() const;
    void setVersion(const QString &version);

    QString description()const;
    void setDescription(const QString &description);

    QString file()const;
    void setFile(const QString &file);

    Q_INVOKABLE void setIniFile();

signals:
    void typeChanged();
    void nameChanged();
    void versionChanged();
    void descriptionChanged();
    void fileChanged();

private:
    QString m_type;
    QString m_name;
    QString m_version;
    QString m_description;
    QString m_file;
};

#endif // PLUGININISETTER_H
