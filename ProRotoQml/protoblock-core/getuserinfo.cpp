#include "getuserinfo.h"
#include "Commissioner.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDirIterator>
#include "FantasyAgent.h"
using namespace fantasybit;

GetUserInfo::GetUserInfo(QObject *parent) : QObject(parent) ,
    m_fantasy_agent{},
    m_socketc(QUrl(QStringLiteral("ws://127.0.0.1:4000")),true)
{
    QString dir = QString::fromStdString (GET_ROOT_DIR()).append ("/secert3.out");
    setsecert3File(dir);
    setSecertFile (dir);

}

GetUserInfo::Type GetUserInfo::type() const
{
    return m_type;
}

void GetUserInfo::setType(const GetUserInfo::Type &type)
{
    if(m_type == type){
        return ;
    }
    else
    {
        switch(type){
        case Name:
            m_type = Name;
            break;
        case PublicKey :
            m_type = PublicKey;
            break;
        case Mnemonic:
            m_type = Mnemonic;
            break;
        case PrivateKey:
            m_type = PrivateKey;
            break;
        }
    }
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
QString GetUserInfo::importMnemonic(const QString &importStr)
{
    auto mfn = m_fantasy_agent.UseMnemonic(importStr.toStdString());

    if ( mfn.status() == MyNameStatus::confirmed ) {
//        emit ImportSucess(QString(mfn.name().data()));
//        useNewName = mfn;
        error(QString("\"%1\" imported!").arg(mfn.name().data()));

    }
    else
        error(QString("import failed. please input valid secret"));

    return "Fuck";

}



fantasybit::MyFantasyName GetUserInfo::iMnemonic(std::string &in) {
//    return agentdata.UseMnemonic(in);
    return fantasybit::MyFantasyName{};
}

//QString GetUserInfo::clicked()  {
//    std::string text = "grape feature three frozen slide sprint scheme plow skate umbrella prison good";

//    auto hseed = mnemonicToSeed(text);
//    pb::sha256 secret;
//    // 32 bytes is 256bits
//    memcpy(secret.data, hseed.data(), 32);
//    auto pk = pb::secp256k1_privkey::regenerate(secret.str());
//    auto pubk =  pk.get_public_key().serialize ();
//    auto tostr =  pb::to_base58(pubk);
//    QString ret = QString::fromStdString (tostr);
//    //    qDebug() << ret;
//    return  ret;
//}

void GetUserInfo::signPlayer(const QString &name)  {
    m_fantasy_agent.signPlayer(name.toStdString());
    NameTrans nt{};
    nt.set_public_key(m_fantasy_agent.pubKeyStr());
    nt.set_fantasy_name(name.toStdString());


    Transaction trans{};
    trans.set_version(Commissioner::TRANS_VERSION);
    trans.set_type(TransType::NAME);
    trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
    SignedTransaction sn = m_fantasy_agent.makeSigned(trans);
    auto txstr = sn.SerializeAsString();

    QByteArray qb(txstr.data(),(size_t)txstr.size());

    m_socketc.sendBinaryMessage(qb);
    //m_fantasy_agent.onSignedTransaction(sn);
//    DoSubscribe(myCurrentName.name(),true);
//    DoPostTx(sn);
}

void GetUserInfo::handdleError(const QString err)
{
    if ( m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();

}
