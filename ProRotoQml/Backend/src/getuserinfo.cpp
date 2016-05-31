#include "getuserinfo.h"

GetUserInfo::GetUserInfo(QObject *parent) : QObject(parent)
{
connect (this,SIGNAL (error(QString)),this,SLOT (handdleError(QString)));
}

QStringList GetUserInfo::fantasyName()
{
    return m_fantasyName;
}

void GetUserInfo::setFantasyName(const QString &fantasyName)
{
    if ( m_fantasyName.contains(fantasyName))
        return;
    m_fantasyName << fantasyName;
    emit fantasyNameChanged();
}

QStringList GetUserInfo::mnemonicKey()
{
    return m_mnemonicKey;
}

void GetUserInfo::setMnemonicKey(const QString &mnemonicKey)
{
    if ( m_mnemonicKey.contains (mnemonicKey))
        return;
    m_mnemonicKey << mnemonicKey;
    emit mnemonicKeyChanged();

}

QStringList GetUserInfo::privateKey()
{
    return m_privateKey;
}

void GetUserInfo::setPrivateKey(const QString &privateKey)
{
    if ( m_privateKey.contains (privateKey))
        return;
    m_privateKey << privateKey;
    emit privateKeyChanged();
}

QStringList GetUserInfo::publicKey()
{
    return m_publicKey;
}

void GetUserInfo::setPublicKey(const QString &publicKey)
{
    if ( m_publicKey.contains (publicKey))
        return;
    m_publicKey <<  publicKey;
    emit publicKeyChanged();
}

QString GetUserInfo::secertFile() const
{
    return m_secertFile;
}

void GetUserInfo::setSecertFile(const QString &secertFile)
{
    if ( m_secertFile == secertFile)
        return;
    m_secertFile = secertFile;
    emit secertFileChanged();

}

QString GetUserInfo::errorString() const
{
    return m_errorString;
}

void GetUserInfo::readData()
{
    if(m_secertFile == ""){
        error("can not open a empty file path");
        return;
    }
    fantasybit::Reader2 read{m_secertFile.toStdString ()};
    if ( !read.good() ) {
                error("Could not open file to read!");
                return;
    }

    fantasybit::Secret3 secret;

    while (read.ReadNext(secret)) {

        setFantasyName (secret.fantasy_name().data ());
        setMnemonicKey (secret.mnemonic_key ().data());
        setPrivateKey (secret.private_key ().data ());
        setPublicKey (secret.public_key ().data ());


        if ( secret.has_mnemonic_key() ) {
            secret.clear_mnemonic_key();
            secret.clear_private_key ();
        }

    }
    emit done();

}




/*!
 * \brief GetUserInfo::importMnemonic
 * \param importStr
 * \return
 * This will return the mn for import
 */
QString GetUserInfo::importMnemonic(QString importStr)
{

    //void fnametool::on_importButton_clicked()
    //{

////       auto mn =  ui->importMnemonic->toPlainText();
//       auto bs =  importStr.toStdString();
//       auto mfn = iMnemonic(bs);

//       if ( mfn.status() == fantasybit::MyNameStatus::confirmed  ) {
//           useNewName = mfn;
//           QMessageBox::information(this,APPLICATION_NAME,
//                                    QString("\"%1\" imported!").arg(mfn.name().data()));

//           this->close();
//       }
//       else
//           QMessageBox::information(this,APPLICATION_NAME,
//                           QString("import failed. please input valid secret"));


    return "Fuck";

    }



fantasybit::MyFantasyName GetUserInfo::iMnemonic(std::string &in) {
//    return agentdata.UseMnemonic(in);
    return fantasybit::MyFantasyName{};
}

void GetUserInfo::handdleError(const QString err)
{
    if ( m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();

}
