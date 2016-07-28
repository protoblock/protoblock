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


class GetUserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString secertFile READ secertFile WRITE setSecertFile NOTIFY secertFileChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    Q_PROPERTY(QStringList fantasyName READ fantasyName NOTIFY fantasyNameChanged)
    Q_PROPERTY(QStringList mnemonicKey READ mnemonicKey NOTIFY mnemonicKeyChanged)
    Q_PROPERTY(QStringList privateKey READ privateKey  NOTIFY privateKeyChanged)
    Q_PROPERTY(QStringList publicKey READ publicKey NOTIFY publicKeyChanged)

public:
    explicit GetUserInfo(QObject *parent = 0);
//    fantasybit::FantasyAgent agentdata;

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
    Q_INVOKABLE QString importMnemonic(QString importStr);
    fantasybit::MyFantasyName iMnemonic(std::string &in);


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
