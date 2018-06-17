#ifndef SECURENODES_H
#define SECURENODES_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <memory>
#include "botan.h"


using namespace std;
using namespace Botan;

class SecureNodes : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE     setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString directory READ directory WRITE setDirectory NOTIFY directoryChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool fullDirectory READ fullDirectory WRITE setFullDirectory NOTIFY fullDirectoryChanged)
    Q_PROPERTY(bool removeAfterEncyption READ removeAfterEncyption WRITE setRemoveAfterEncyption NOTIFY removeAfterEncyptionChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
//    Q_PROPERTY(bool checkMasterPassword READ checkMasterPassword WRITE setCheckMasterPassword NOTIFY checkMasterPasswordChanged)

public:
    explicit SecureNodes(QObject *parent = 0);

    QString hash(const QString data);
    QString hexHash(const QString data);
    QString encode(const QString data);
    QString decode(const QString data);
    QString encrypt(const QString data);
    QString decrypt(const QString data);
    Q_INVOKABLE bool encryptFile( const  QString destination);
    Q_INVOKABLE bool decryptFile(const QString source, const QString destination);
//    void createMnemonic();


    QString password()const;
    void setPassword(const QString &password);
    void setSalt(const QString salt) ;

    Q_INVOKABLE bool checkMasterPassword(const QString &pass);

    QString directory()const;
    void setDirectory(const QString &directory);

    QString fileName()const;
    void setFileName(const QString &fileName);

    bool fullDirectory()const;
    void setFullDirectory(const bool &fullDirectory);

    bool removeAfterEncyption()const;
    void setRemoveAfterEncyption(const bool &removeAfterEncyption);

    QString errorString()const;

signals:
    void passwordChanged();
    void directoryChanged();
    void fileNameChanged();
    void fullDirectoryChanged();
    void removeAfterEncyptionChanged();
    void error(QString);
    void errorStringChanged();

public slots:
    void handelError(const QString &err);

private:
    Botan::LibraryInitializer mInit;
    SecureVector<byte> m_salt;
    QString m_password;
    QString m_directory;
    QString m_fileName;
    bool m_fullDirectory;
    bool m_removeAfterEncyption;
    QString m_errorString;
};

#endif // SecureNodes_H
