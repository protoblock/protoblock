#include "securenodes.h"

SecureNodes::SecureNodes(QObject *parent) :
    QObject(parent),
    m_removeAfterEncyption(false)
{
    //Set the default salt size
    m_salt.resize(48);

    //Set the default salt
    int saltSize = m_salt.size();
    for(int i = 0; i < saltSize; i++)
    {
        m_salt[i] = i;
    }

   connect(this,SIGNAL(error(QString)),this,SLOT(handelError(QString)) ) ;
}

QString SecureNodes::hash(const QString data)
{
    try
    {
        Pipe pipe(new Hash_Filter("SHA-1"));
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}

QString SecureNodes::hexHash(const QString data)
{
    try
    {
        Pipe pipe(new Hash_Filter("SHA-1"), new Hex_Encoder);
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}

QString SecureNodes::encode(const QString data)
{
    try
    {
        Pipe pipe(new Base64_Encoder);
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}

QString SecureNodes::decode(const QString data)
{
    try
    {
        Pipe pipe(new Base64_Decoder);
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}

QString SecureNodes::encrypt(const QString data)
{
    try
    {
        //Setup the key derive functions
        PKCS5_PBKDF2 pbkdf2(new HMAC(new SHA_160));
        const u32bit PBKDF2_ITERATIONS = 8192;

        //Create the KEY and IV
        KDF* kdf = get_kdf("KDF2(SHA-1)");

        //Create the master key
        SecureVector<byte> m_master = pbkdf2.derive_key(48, m_password.toStdString(), &m_salt[0], m_salt.size(),PBKDF2_ITERATIONS).bits_of();
        SymmetricKey m_key = kdf->derive_key(32, m_master, "salt1");
        InitializationVector mIV = kdf->derive_key(16, m_master, "salt2");

        Pipe pipe(get_cipher("AES-256/CBC/PKCS7", m_key, mIV,ENCRYPTION),new Base64_Encoder);
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}

QString SecureNodes::decrypt(const QString data)
{
    try
    {
        //Setup the key derive functions
        PKCS5_PBKDF2 pbkdf2(new HMAC(new SHA_160));
        const u32bit PBKDF2_ITERATIONS = 8192;

        //Create the KEY and IV
        KDF* kdf = get_kdf("KDF2(SHA-1)");

        //Create the master key
        SecureVector<byte> m_master = pbkdf2.derive_key(48, m_password.toStdString(), &m_salt[0],
                m_salt.size(),PBKDF2_ITERATIONS).bits_of();
        SymmetricKey m_key = kdf->derive_key(32, m_master, "salt1");
        InitializationVector mIV = kdf->derive_key(16, m_master, "salt2");

        Pipe pipe(new Base64_Decoder,get_cipher("AES-256/CBC/PKCS7", m_key, mIV,DECRYPTION));
        pipe.process_msg(data.toStdString());
        QString value = QString::fromStdString(pipe.read_all_as_string(0));
        return value;
    }
    catch(...)
    {
        return "";
    }
}










bool SecureNodes::encryptFile(const QString destination)
{
    try
    {
        //Setup the key derive functions
        PKCS5_PBKDF2 pbkdf2(new HMAC(new SHA_160));
        const u32bit PBKDF2_ITERATIONS = 8192;

        //Create the KEY and IV
        KDF* kdf = get_kdf("KDF2(SHA-1)");

        //Create the master key
        SecureVector<byte> m_master = pbkdf2.derive_key(48, m_password.toStdString(), &m_salt[0], m_salt.size(),PBKDF2_ITERATIONS).bits_of();
        SymmetricKey m_key = kdf->derive_key(32, m_master, "salt1");
        InitializationVector mIV = kdf->derive_key(16, m_master, "salt2");

        string inFilename = m_fileName.toStdString();
        string outFilename = destination.toStdString();
        std::ifstream in(inFilename.c_str(),std::ios::binary);
        std::ofstream out(outFilename.c_str(),std::ios::binary);

        Pipe pipe(get_cipher("AES-256/CBC/PKCS7", m_key, mIV,ENCRYPTION),new DataSink_Stream(out));
        pipe.start_msg();
        in >> pipe;
        pipe.end_msg();

        out.flush();
        out.close();
        in.close();
        QFile m_file(m_fileName);
        if(m_removeAfterEncyption == true){
//            qDebug() << "remove after encryption called ";
            if(m_file.exists())
            {
//                qDebug() << "trying to remove the file " << m_fileName;
                if(!m_file.open(QIODevice::ReadWrite|QFile::Truncate))
                {
                        error("could not open the file for truncating");
//                    m_file.remove();
                }
                else
                {
                    if (m_file.remove())
                    {
//                        qDebug() << "File Removed !!";
                    }
                    else
                    {
                        error("Could not remove file ") ;// << m_fileName;
                    }
                }
            }
            else
            {
                qDebug() << "file name is not exists ";
            }
        }
        else
        {
//           error("Seems like remove after is not called");
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool SecureNodes::decryptFile(const QString source,const QString destination)
{
    try
    {
        //Setup the key derive functions
        PKCS5_PBKDF2 pbkdf2(new HMAC(new SHA_160));
        const u32bit PBKDF2_ITERATIONS = 8192;

        //Create the KEY and IV
        KDF* kdf = get_kdf("KDF2(SHA-1)");

        //Create the master key
        SecureVector<byte> m_master = pbkdf2.derive_key(48, m_password.toStdString(), &m_salt[0], m_salt.size(),PBKDF2_ITERATIONS).bits_of();
        SymmetricKey m_key = kdf->derive_key(32, m_master, "salt1");
        InitializationVector mIV = kdf->derive_key(16, m_master, "salt2");

        string inFilename = source.toStdString();
        string outFilename = destination.toStdString();
        std::ifstream in(inFilename.c_str(),std::ios::binary);
        std::ofstream out(outFilename.c_str(),std::ios::binary);

        Pipe pipe(get_cipher("AES-256/CBC/PKCS7", m_key, mIV,DECRYPTION),new DataSink_Stream(out));
        pipe.start_msg();
        in >> pipe;
        pipe.end_msg();

        out.close();
        out.flush();
        in.close();

        return true;
    }
    catch(...)
    {
        return false;
    }
}

QString SecureNodes::password() const
{
    return m_password;
}
void SecureNodes::setPassword(const QString &password)
{
    //Set the password
    if(m_password == password)
        return ;
    m_password = hash(password);
    emit passwordChanged();
}

void SecureNodes::setSalt(const QString salt)
{
    QByteArray cBytes = salt.toLatin1();

    int saltSize = m_salt.size();

    for(int i = 0;i < saltSize; i++)
    {
        m_salt[i] = cBytes[i];
    }
}

bool SecureNodes::checkMasterPassword(const QString &pass)
{
    if(m_password == pass){
        return true;
    }else{
        return false;
    }
}

QString SecureNodes::directory() const
{
    return m_directory;
}

void SecureNodes::setDirectory(const QString &directory)
{
    if(m_directory == directory)
        return;
    m_directory = directory;
    emit directoryChanged();

}

QString SecureNodes::fileName() const
{
    return m_fileName;
}

void SecureNodes::setFileName(const QString &fileName)
{
    if(m_fileName == fileName)
        return;
    m_fileName = fileName;
    emit fileNameChanged();
}

bool SecureNodes::fullDirectory() const
{
    return m_fullDirectory;
}

void SecureNodes::setFullDirectory(const bool &fullDirectory)
{
    if(m_fullDirectory == fullDirectory)
        return;
    m_fullDirectory = fullDirectory;
    emit fullDirectoryChanged();
}

bool SecureNodes::removeAfterEncyption() const
{
    return m_removeAfterEncyption;
}

void SecureNodes::setRemoveAfterEncyption(const bool &removeAfterEncyption)
{
    if(m_removeAfterEncyption == removeAfterEncyption)
        return;
    m_removeAfterEncyption = removeAfterEncyption;
    emit removeAfterEncyptionChanged();
}

QString SecureNodes::errorString() const
{
    return m_errorString;
}

void SecureNodes::handelError(const QString &err)
{
    if(m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();
}
