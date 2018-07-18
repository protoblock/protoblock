#ifndef GENERATEKEYS_H
#define GENERATEKEYS_H
#include "ssh_global.h"
#include "QObject"
#include <QCoreApplication>
#include <QSharedPointer>


#include "sshbotanconversions_p.h"
#include "sshcapabilities_p.h"
#include "ssh_global.h"
#include "sshinit_p.h"
#include "sshpacket_p.h"

#include <botan.h>

#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

#include <string>



namespace Botan {
class Private_Key;
class RandomNumberGenerator;
}


class GenerateKeys : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString getPassword READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(int keySize READ keySize WRITE setKeySize NOTIFY keySizeChanged)
    Q_PROPERTY(QString privateKeyFilePath READ privateKeyFilePath WRITE setPrivateKeyFilePath NOTIFY privateKeyFilePathChanged)
    Q_PROPERTY(QString publicKeyFilePath READ publicKeyFilePath WRITE setPublicKeyFilePath NOTIFY publicKeyFilePathChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    Q_PROPERTY(KeyType keyType READ keyType WRITE setKeyType NOTIFY keyTypeChanged)
    Q_PROPERTY(PrivateKeyFormat format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(EncryptionMode encryptionMode READ encryptionMode WRITE setEncryptionMode)
    Q_ENUMS(KeyType)
    Q_ENUMS(PrivateKeyFormat)
    Q_ENUMS(EncryptionMode)

public:
    explicit GenerateKeys(QObject *parent = 0);

    enum KeyType{ Rsa, Dsa, Ecdsa };
    KeyType keyType() const;
    void setKeyType(const KeyType &keyType);


    enum PrivateKeyFormat { Pkcs8, OpenSsl, Mixed };
    PrivateKeyFormat format()const;
    void setFormat(const PrivateKeyFormat &format);


    enum EncryptionMode { DoOfferEncryption, DoNotOfferEncryption };
    EncryptionMode encryptionMode() const;
    void setEncryptionMode(const EncryptionMode &encryptionMode);

    QString errorString()const;


    //FIXME hardcode in the vals for this later on QStringList for models
    //    keySizes << QLatin1String("1024") << QLatin1String("2048") << QLatin1String("4096");
    //    ecdsa
    //    keySizes << QLatin1String("256") << QLatin1String("384") << QLatin1String("521");
    int keySize() const;
    void setKeySize(int &keySize);


    QString getPassword() const;
    void setPassword(const QString &password);


    QString privateKeyFilePath() const;
    void setPrivateKeyFilePath(const QString &privateKeyFilePath);

    QString publicKeyFilePath() const;
    void setPublicKeyFilePath(const QString &publicKeyFilePath);

    bool generateKeys();
    Q_INVOKABLE void generateAndSaveKeys();

    // this can be used many different ways
    QByteArray privateKey() const { return m_privateKey; }
    QByteArray publicKey() const { return m_publicKey; }
    KeyType type() const { return m_keyType; }


    int size_t2int(size_t val) {
        return (val <= INT_MAX) ? (int)((ssize_t)val) : -1;
    }

    size_t int2size_t(int val) {
        return (val < 0) ? __SIZE_MAX__ : (size_t)((unsigned)val);
    }

signals:
    void passwordChanged();
    void keySizeChanged();
    void keyTypeChanged();
    void formatChanged();
    void encryptionModeChanged();
    void error(QString);
    void errorStringChanged();
    void publicKeyFilePathChanged();
    void privateKeyFilePathChanged();
    void done();
    void failed();
    void started();

protected slots:
    void handleError(const QString err);

private:
    void saveKeys();
    bool userForbidsOverwriting();

    typedef QSharedPointer<Botan::Private_Key> KeyPtr;
    void generatePkcs8KeyStrings(const KeyPtr &key, Botan::RandomNumberGenerator &rng);
    void generatePkcs8KeyString(const KeyPtr &key, bool privateKey,
                                Botan::RandomNumberGenerator &rng);
    void generateOpenSslKeyStrings(const KeyPtr &key);
    void generateOpenSslPrivateKeyString(const KeyPtr &key);
    void generateOpenSslPublicKeyString(const KeyPtr &key);

    QString m_errorString;
    QString m_password;
    KeyType m_keyType;
    PrivateKeyFormat m_format;
    EncryptionMode m_encryptionMode;

    int m_keySize;
    QString m_publicKeyFilePath;
    QString m_privateKeyFilePath;
    QByteArray m_privateKey;
    QByteArray m_publicKey;
};


#endif // GENERATEKEYS_H
