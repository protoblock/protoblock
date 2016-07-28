#ifndef GETUSERINFO_H
#define GETUSERINFO_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <fstream>
#include <iostream>
#include <ios>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message.h>
#include <fcntl.h>

#include "ProtoData.pb.h"
//#include "ProtoData.pb.h"

#include "mnemonic.h"
#include <QDebug>
#include <string>

#include "FantasyAgent.h"
#include "QQmlConstRefPropertyHelpers.h"
#include "socketclient.h"

class GetUserInfo : public QObject
{
    Q_OBJECT

    QML_READONLY_CSTREF_PROPERTY (QString, secert3File)


    Q_PROPERTY(QString secertFile READ secertFile WRITE setSecertFile NOTIFY secertFileChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    // we use Qstring beacuas the JS engine is faster to process these. tehn a variant or auto can
    // more code yes but faster time is * 800 to teh ^2 of a variant or a auto
    // well refernced
    Q_PROPERTY(QStringList fantasyName READ fantasyName NOTIFY fantasyNameChanged)
    Q_PROPERTY(QStringList mnemonicKey READ mnemonicKey NOTIFY mnemonicKeyChanged)
    Q_PROPERTY(QStringList privateKey READ privateKey  NOTIFY privateKeyChanged)
    Q_PROPERTY(QStringList publicKey READ publicKey NOTIFY publicKeyChanged)

    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)

    Q_ENUMS (Type)
public:
    explicit GetUserInfo(QObject *parent = 0);
//    fantasybit::FantasyAgent agentdata;

    enum Type{
       Name,
       PublicKey,
       Mnemonic,
       PrivateKey
    };

    Type type()const;
    void setType(const Type &type);

    QStringList fantasyName();
    void setFantasyName(const QString &fantasyName);

    QStringList mnemonicKey () ;
    void setMnemonicKey(const QString &mnemonicKey);

    QStringList privateKey ();
    void setPrivateKey(const QString &privateKey);

    QStringList publicKey ();
    void setPublicKey(const QString &publicKey);

    QString secertFile()const;
    void setSecertFile(const QString &secertFile );

    QString errorString()const;

    Q_INVOKABLE void readData();
    Q_INVOKABLE QString importMnemonic(const QString &importStr);


//    Q_INVOKABLE
    fantasybit::MyFantasyName iMnemonic(std::string &in);

    Q_INVOKABLE void signPlayer(const QString& name);


signals:
    void secertFileChanged();
    void error(QString);
    void errorStringChanged();

    void fantasyNameChanged();
    void mnemonicKeyChanged();
    void privateKeyChanged();
    void publicKeyChanged();

    void running();
    void done();
    void typeChanged();

protected slots:
    void handdleError(const QString err);
private:
    QStringList m_fantasyName ;
    QStringList m_mnemonicKey ;
    QStringList m_privateKey ;
    QStringList m_publicKey ;

    QString m_errorString;
    QString m_secertFile;
    fantasybit::Secret m_sec;
    fantasybit::FantasyAgent m_fantasy_agent;

    SocketClient m_socketc;

    Type m_type;

};

#endif // GETUSERINFO_H
namespace fantasybit
{

class Reader2
{
    std::ifstream mFs;
    GOOGLE_NAMESPACE::protobuf::io::IstreamInputStream *_IstreamInputStream = nullptr;
    GOOGLE_NAMESPACE::protobuf::io::CodedInputStream *_CodedInputStream = nullptr;
    bool gd = false;

public:
    Reader2(const std::string &file, std::ios::openmode flags = std::ios::in):
        mFs(file,std::ios::in | std::ios::binary | flags)
    {
        if (mFs.good())
        {
            gd = true;
            _IstreamInputStream = new GOOGLE_NAMESPACE::protobuf::io::IstreamInputStream(&mFs);
            _CodedInputStream = new GOOGLE_NAMESPACE::protobuf::io::CodedInputStream(_IstreamInputStream);
        }
    }

    bool good() const {
        return gd;
    }

    bool ReadNext(GOOGLE_NAMESPACE::protobuf::Message &msg)
    {
        if ( !good() )
        {
            std::cerr << "Reader<> error !good\n";
            return false;
        }

        unsigned int size;

        bool ret;
        if ( (ret = _CodedInputStream->ReadVarint32(&size)) )
        {
            GOOGLE_NAMESPACE::protobuf::io::CodedInputStream::Limit msgLimit = _CodedInputStream->PushLimit(size);
            ret = msg.ParseFromCodedStream(_CodedInputStream);
            _CodedInputStream->PopLimit(msgLimit);
        }

        return ret;
    }

    ~Reader2()
    {
        if (_CodedInputStream != nullptr )
            delete _CodedInputStream;
        if (_IstreamInputStream != nullptr)
            delete _IstreamInputStream;
        mFs.close();
    }
};
}
