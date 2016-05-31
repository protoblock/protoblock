#include "generatekeys.h"


using namespace Botan;
using namespace QSsh;
using namespace QSsh::Internal;

GenerateKeys::GenerateKeys(QObject *parent) :
    QObject(parent),
    m_keyType(Ecdsa),
    m_format(Pkcs8),
    m_encryptionMode(DoOfferEncryption),
    m_keySize(521)

{
    initSsh();
    //FIXME make this into application path ?
    const QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+ QLatin1String("/.ssh/proroto_id");
//    qDebug( ) << defaultPath;
    setPrivateKeyFilePath (defaultPath);
    setPublicKeyFilePath (QString("%1.pub").arg (defaultPath));
    connect (this,SIGNAL (error(QString)),
             this,SLOT(handleError (QString)));

}



GenerateKeys::KeyType GenerateKeys::keyType() const{
    return m_keyType;
}


/*!
 * \brief GenerateKeys::setKeyType
 * \param keyType
 * This sets the type of key that one would like to use for there private and public keys
 */
void GenerateKeys::setKeyType(const GenerateKeys::KeyType &keyType)
{
    if (m_keyType == keyType)
    {
        return;
    }
    else
    {
        if (keyType == Rsa){
            m_keyType = Rsa;
        }
        else if (keyType  == Dsa)
        {
            m_keyType = Dsa;
        }
        //FIXME lets make for Ecdh and also
        else if (keyType == Ecdsa)
        {
            m_keyType = Ecdsa;
        }
        emit keyTypeChanged();
    }
}

GenerateKeys::PrivateKeyFormat GenerateKeys::format() const
{
    return m_format;
}

void GenerateKeys::setFormat(const GenerateKeys::PrivateKeyFormat &format)
{
    if (m_format == format)
    {
        return;
    }
    else
    {
        if( format== Pkcs8)
        {
            m_format = Pkcs8;
        }
        else if (format == OpenSsl)
        {
            m_format = OpenSsl;
        }
        else if (format == Mixed)
        {
            m_format = Mixed;
        }
        emit formatChanged();
    }
}

/*!
 * \brief GenerateKeys::keySize
 * \return
    reads the size that the key algo will be used with
*/
int GenerateKeys::keySize() const
{
    return m_keySize;
}

void GenerateKeys::setKeySize(int &keySize)
{
    if(m_keySize == keySize)
        return;
    m_keySize = keySize;
    emit keySizeChanged();
}

GenerateKeys::EncryptionMode GenerateKeys::encryptionMode() const
{
    return m_encryptionMode;
}

void GenerateKeys::setEncryptionMode(const GenerateKeys::EncryptionMode &encryptionMode)
{
    if(m_encryptionMode == encryptionMode)
    {
        return;
    }
    else
    {
        if(encryptionMode == DoNotOfferEncryption)
        {
            m_encryptionMode = DoNotOfferEncryption;
        }
        else if (encryptionMode == DoOfferEncryption)
        {
            m_encryptionMode = DoOfferEncryption;
        }
        emit encryptionModeChanged();
    }
}

QString GenerateKeys::errorString() const
{
    return m_errorString;
}

QString GenerateKeys::getPassword() const
{
    return m_password;
}
void GenerateKeys::setPassword(const QString &password)
{
    if(m_password == password)
        return;
    m_password = password;
    emit passwordChanged();
}

QString GenerateKeys::privateKeyFilePath() const
{
    return m_privateKeyFilePath;
}

void GenerateKeys::setPrivateKeyFilePath(const QString &privateKeyFilePath)
{
    if(m_privateKeyFilePath == privateKeyFilePath)
        return;
    m_privateKeyFilePath = privateKeyFilePath;
    emit privateKeyFilePathChanged();
}

QString GenerateKeys::publicKeyFilePath() const
{
    return m_publicKeyFilePath;
}

void GenerateKeys::setPublicKeyFilePath(const QString &publicKeyFilePath)
{
    if(m_publicKeyFilePath == publicKeyFilePath)
        return;
    m_publicKeyFilePath = publicKeyFilePath;
    emit publicKeyFilePathChanged();
}


bool GenerateKeys::generateKeys()
{
    try {
        AutoSeeded_RNG rng;
        KeyPtr key;
        switch (m_keyType) {
        case Rsa:
            // FIXME make m_keySize a size_t
            key = KeyPtr(new RSA_PrivateKey(rng, m_keySize));
            break;
        case Dsa:
            key = KeyPtr(new DSA_PrivateKey(rng, DL_Group(rng, DL_Group::DSA_Kosherizer, m_keySize)));
            break;
        case Ecdsa: {
            const QByteArray algo = SshCapabilities::ecdsaPubKeyAlgoForKeyWidth(m_keySize / 8);
            key = KeyPtr(new ECDSA_PrivateKey(rng, EC_Group(SshCapabilities::oid(algo))));
            break;
        }
        }
        switch (m_format) {
        case Pkcs8:
            generatePkcs8KeyStrings(key, rng);
            break;
        case OpenSsl:
            generateOpenSslKeyStrings(key);
            break;
        case Mixed:
        default:
            generatePkcs8KeyString(key, true, rng);
            generateOpenSslPublicKeyString(key);
        }
        return true;
    } catch (const Exception &e) {
        qDebug() << QString("Error generating key: %1").arg(QString::fromLatin1(e.what()));
        return false;
    }
}

void GenerateKeys::generatePkcs8KeyStrings(const KeyPtr &key, RandomNumberGenerator &rng)
{
    generatePkcs8KeyString(key, false, rng);
    generatePkcs8KeyString(key, true, rng);
}

void GenerateKeys::generatePkcs8KeyString(const KeyPtr &key, bool privateKey,
                                          RandomNumberGenerator &rng)
{
    Pipe pipe;
    pipe.start_msg();
    QByteArray *keyData;
    if (privateKey) {
        QString password;
        if (m_encryptionMode == DoOfferEncryption)
            password = getPassword();
        if (!password.isEmpty())
            pipe.write(PKCS8::PEM_encode(*key, rng, password.toLocal8Bit().data()));
        else
            pipe.write(PKCS8::PEM_encode(*key));
        keyData = &m_privateKey;
    } else {
        pipe.write(X509::PEM_encode(*key));
        keyData = &m_publicKey;
    }
    pipe.end_msg();
    keyData->resize(pipe.remaining(pipe.message_count() - 1));
    pipe.read(convertByteArray(*keyData), keyData->size(),
              pipe.message_count() - 1);
}

void GenerateKeys::generateOpenSslKeyStrings(const KeyPtr &key)
{
    generateOpenSslPublicKeyString(key);
    generateOpenSslPrivateKeyString(key);
}

void GenerateKeys::generateOpenSslPublicKeyString(const KeyPtr &key)
{
    QList<BigInt> params;
    QByteArray keyId;
    QByteArray q;
    switch (m_keyType) {
    case Rsa: {
        const QSharedPointer<RSA_PrivateKey> rsaKey = key.dynamicCast<RSA_PrivateKey>();
        params << rsaKey->get_e() << rsaKey->get_n();
        keyId = SshCapabilities::PubKeyRsa;
        break;
    }
    case Dsa: {
        const QSharedPointer<DSA_PrivateKey> dsaKey = key.dynamicCast<DSA_PrivateKey>();
        params << dsaKey->group_p() << dsaKey->group_q() << dsaKey->group_g() << dsaKey->get_y();
        keyId = SshCapabilities::PubKeyDss;
        break;
    }
    case Ecdsa: {
        const auto ecdsaKey = key.dynamicCast<ECDSA_PrivateKey>();
        q = convertByteArray(EC2OSP(ecdsaKey->public_point(), PointGFp::UNCOMPRESSED));
        keyId = SshCapabilities::ecdsaPubKeyAlgoForKeyWidth(ecdsaKey->private_value().bytes());
        break;
    }
    }

    QByteArray publicKeyBlob = AbstractSshPacket::encodeString(keyId);
    foreach (const BigInt &b, params)
        publicKeyBlob += AbstractSshPacket::encodeMpInt(b);
    if (!q.isEmpty()) {
        publicKeyBlob += AbstractSshPacket::encodeString(keyId.mid(11)); // Without "ecdsa-sha2-" prefix.
        publicKeyBlob += AbstractSshPacket::encodeString(q);
    }
    publicKeyBlob = publicKeyBlob.toBase64();
    const QByteArray id = "ProRoto/"
            + QDateTime::currentDateTime().toString(Qt::ISODate).toUtf8();
    m_publicKey = keyId + ' ' + publicKeyBlob + ' ' + id;
}

void GenerateKeys::generateOpenSslPrivateKeyString(const KeyPtr &key)
{
    QList<BigInt> params;
    const char *label = "";
    switch (m_keyType) {
    case Rsa: {
        const QSharedPointer<RSA_PrivateKey> rsaKey
                = key.dynamicCast<RSA_PrivateKey>();
        params << rsaKey->get_n() << rsaKey->get_e() << rsaKey->get_d() << rsaKey->get_p()
               << rsaKey->get_q();
        const BigInt dmp1 = rsaKey->get_d() % (rsaKey->get_p() - 1);
        const BigInt dmq1 = rsaKey->get_d() % (rsaKey->get_q() - 1);
        const BigInt iqmp = inverse_mod(rsaKey->get_q(), rsaKey->get_p());
        params << dmp1 << dmq1 << iqmp;
        label = "RSA PRIVATE KEY";
        break;
    }
    case Dsa: {
        const QSharedPointer<DSA_PrivateKey> dsaKey = key.dynamicCast<DSA_PrivateKey>();
        params << dsaKey->group_p() << dsaKey->group_q() << dsaKey->group_g() << dsaKey->get_y()
               << dsaKey->get_x();
        label = "DSA PRIVATE KEY";
        break;
    }
    case Ecdsa:
        params << key.dynamicCast<ECDSA_PrivateKey>()->private_value();
        label = "EC PRIVATE KEY";
        break;
    }

    DER_Encoder encoder;
    encoder.start_cons(SEQUENCE).encode(size_t(0));
    foreach (const BigInt &b, params)
        encoder.encode(b);
    encoder.end_cons();
    m_privateKey = QByteArray(PEM_Code::encode (encoder.get_contents(), label).c_str());
}


void GenerateKeys::generateAndSaveKeys()
{
    emit started();
    if (userForbidsOverwriting()){
        error("File already exsists");
        return;
    }
    const bool success = generateKeys();
    if (success)
    {
        saveKeys();
    }
    else
    {
        QString m_er = tr("Key Generation Failed");
        error (m_er);
    }
}

void GenerateKeys::handleError(const QString err)
{
    emit failed();
    if(m_errorString == err)
        return;
    m_errorString = err;
    qDebug() << m_errorString;
    emit errorStringChanged ();
}

void GenerateKeys::saveKeys()
{

    const QString parentDir = QFileInfo(privateKeyFilePath()).dir().path();
    if (!QDir::root().mkpath(parentDir)) {
        QString m_er = tr("Cannot Save Key File Failed to create directory: \"%1\".").arg(parentDir);
        error (m_er);
        return;
    }

    QFile privateKeyFile(privateKeyFilePath());
    if (!privateKeyFile.open(QIODevice::WriteOnly)
            || !privateKeyFile.write( m_privateKey )) {
        QString m_er = tr("Cannot Save Private Key File The private key file could not be saved: %1").arg(privateKeyFile.errorString());
        error (m_er);
        return;
    }

    QFile::setPermissions(privateKeyFilePath(), QFile::ReadOwner | QFile::WriteOwner);

    QFile publicKeyFile(publicKeyFilePath());
    if (!publicKeyFile.open(QIODevice::WriteOnly)
            || !publicKeyFile.write( m_publicKey )) {
        QString m_er = tr("Cannot Save Public Key File The public key file could not be saved: %1").arg(publicKeyFile.errorString());
        error (m_er);
        return;
    }

    //FIXME maybe this should be a bool
    //    accept();
    emit done();
}


//FIXME add a signal slot to expose to Qml On this
bool GenerateKeys::userForbidsOverwriting()
{

    if (!QFile::exists(privateKeyFilePath()) || !QFile::exists(publicKeyFilePath())){
        return false;
        error ("File already is there ! maybe delete it or set the key to be different");
    }
    else
    {
        return true;
    }
}
