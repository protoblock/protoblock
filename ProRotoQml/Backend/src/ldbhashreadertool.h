#ifndef LDBHASHREADERTOOL_H
#define LDBHASHREADERTOOL_H

#include <QObject>
#include <QMap>
#include <QDebug>
#include <QRegExp>

#include <iostream>
#include <google/protobuf/message.h>

#include "leveldb/db.h"
#include "StateData.pb.h"
#include "maps/ldbhashreadermap.h"

//#include "pbjson.hpp"

class LdbHashReaderTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString debugstring READ debugstring)
    Q_PROPERTY(QStringList types READ types)

    leveldb::DB *db;
    leveldb::WriteOptions write_sync{};


    void init() {
        write_sync.sync = true;

        leveldb::Options opt;
        opt.create_if_missing = true;
        opt.error_if_exists = false;

        leveldb::Status status;
        leveldb::DB *ldb;

#ifdef Q_OS_WIN
        status = leveldb::DB::Open(opt, m_path.toStdString(), &ldb);
#else
        status = leveldb::DB::Open(opt, "/Users/satoshi/work/tmp/tmp/meta", &ldb);
#endif

        if (!status.ok()) {
            exit(1);
        }
        db = ldb;
    }

private:
    static LdbHashReaderTool *myInstance;
    explicit LdbHashReaderTool(QObject *parent = 0);


public:
    static LdbHashReaderTool *instance() {
        if (myInstance == NULL) {
            //            std::lock_guard<std::mutex> lock(myMutex);
            //            if (myInstance == NULL)
            myInstance = new LdbHashReaderTool();
        }
        return myInstance;
    }


    Q_INVOKABLE QStringList getChild(const QString nodesParent);


    Q_INVOKABLE qint32 getBkNumber(const QString trmeta = "")
    {
        if ( trmeta != "") {
            fantasybit::TrMeta tr;
            readData (trmeta.toStdString (),tr);
            return tr.blocknum ();
        }

        std::string s = readstr ("blockhead");
        readData (s.data (),bm);
        return bm.blocknum ();
    }


    Q_INVOKABLE QString getBlockNumberViaPervious(const QString prev)
    {



        //fantasybit::TrMeta trMeta;

        return QString("");
    }

    Q_INVOKABLE void jumpToBlock(const QString &blockNumb)
    {
        std::string blkFromQ = blockNumb.toStdString ();
        std::string s = readstr (blkFromQ);
        QString toQS = QString::fromStdString (s);
        getDebugStr (toQS , "BlockMeta" );
    }

    Q_INVOKABLE QString jumpToPbStateId(){
        std::string s = readstr ("blockhead");
        readData ( s , bm );
        readData ( bm.pbstateid ().data (),  pbS);
        return bm.pbstateid ().data ();
    }

    Q_INVOKABLE QString jumpToLeaderBoard(){
        std::string s = readstr ("blockhead");
        readData ( s , bm );
        readData ( bm.pbstateid ().data (),  pbS);
        readData( pbS.leaderboardstateid (),lbM);
        return lbM.fnbalmetaroot ().data() ; // .leaderboardstateid ().data ();
    }


    Q_INVOKABLE QString fnbalRoot()
    {
        std::string s = readstr ("blockhead");
        readData ( s , bm );
        readData ( bm.pbstateid ().data (),  pbS);
        readData(pbS.leaderboardstateid () , lbM);
        return lbM.fnbalmetaroot ().data () ;
    }

    std::string readData(const std::string &id, GOOGLE_NAMESPACE::protobuf::Message &msg) {
        auto str = readstr (id);
        msg.ParseFromString(str);
        return str;
    }

    std::string readstr(const std::string &id) {
        std::string value;
        db->Get(leveldb::ReadOptions(),id,&value);
        return value;
    }


    Q_INVOKABLE void getDebugStr(const QString id,const QString nType) {
        if (nType == "NULL") return;

        m_dstr = doGetDebugStr(id,nType);

        gotDebugStr ();

    }

//    Q_INVOKABLE QString getJsonStr(const QString &id, GOOGLE_NAMESPACE::protobuf::Message *msg) {

//        readData(id.toStdString().data(),*msg);

//        std::string jsonstr;
//        pbjson::pb2json(msg,jsonstr);


//        return jsonstr.data();
//    }

//    Q_INVOKABLE QString getJsonStr(const QString &id, const QString &nType) {

//        if ( typeMap.find(nType) != typeMap.end() ) {
//            auto mm = typeMap.value(nType);
//            return getJsonStr(id,mm);
//        }
//        else return "";

//    }

//    Q_INVOKABLE QString getJsonDrill(const QString &id, const QString nType) {
//        fantasybit::MerkleTree mtree;
//        readData(id.toStdString().data(),mtree);

//        rapidjson::Document jsondoc;
//        rapidjson::Value::AllocatorType &allocator = jsondoc.GetAllocator();
//        jsondoc.SetObject();
//        rapidjson::Value arr(rapidjson::kArrayType);
//        auto mm = typeMap.value(nType);
//        for ( auto nodestr : mtree.leaves()) {
//            readData(nodestr,*mm);
//            rapidjson::Value *v = pbjson::pb2jsonobject(mm,allocator);
//            //arr.AddMember(v,)
//            arr.PushBack(*v,allocator);
//        }

//        rapidjson::Value ss(rapidjson::kStringType);
//        ss.SetString(nType.toStdString().data(),allocator);

//        jsondoc.AddMember(ss,arr, allocator);
//        std::string jsonstr;
//        pbjson::json2string(&jsondoc,jsonstr);


//        return jsonstr.data();
//    }




    QString doGetDebugStr(const QString id,const QString nType) {
        QString ret;

        if (nType == "NULL" || id == "") return ret;

        else if ( nType == "string") {
            auto str = readstr(id.toStdString());
            ret = str.data();
        }

        else if ( typeMap.find(nType) != typeMap.end() ) {
            auto str = readstr(id.toStdString());
            auto mm = typeMap.value(nType);
            mm->ParseFromString(str);
            ret = typeMap[nType]->DebugString().data();

            ret += getExtra(nType,mm);
        }
        else ret += getExtra(id,nType);

        return ret;
    }

    QString getExtra(const QString &id, const QString &nType) {
        if ( id == "" ) return "";

        if ( nType == "market" ) {
            fantasybit::PlayerMarketState pms;
            readData(id.toStdString(),pms);
            QString ret = pms.DebugString().data();
            return ret += getExtra(pms);
        }
        if ( nType == "limitbook" ) {
            fantasybit::MerkleTree mt;
            readData(id.toStdString(),mt);
            int i = 1;
            QString ret, other;
            for ( auto l : mt.leaves()) {
                fantasybit::LimitBookMeta lbm;
                readData(l,lbm);
                QString a = "level%1: \"%2 %3 %4 %5\"\n";
                ret.append(a.arg(QString::number(i++),
                                 QString::number(lbm.bidsize()),
                                 QString::number(lbm.bid()),

                                 QString::number(lbm.ask()),
                                 QString::number(lbm.asksize())
                                 ));
                other += getExtra(lbm.bidordermetaroot().data(),"orders");
                other += getExtra(lbm.askordermetaroot().data(),"orders");
            }
            return ret += other;
        }
        if ( nType == "orders" ) {
            fantasybit::MerkleTree mt;
            readData(id.toStdString(),mt);
            QString ret;
            for ( auto l : mt.leaves()) {
                QString a = l.data();
                ret += a.append(" {\n") + doGetDebugStr(l.data(),"OrderMeta").append("}\n");
            }
            return ret;
        }
        if ( nType == "markettic" ) {
            return  doGetDebugStr(id,"MarketTicMeta");
        }
        if ( nType == "order" ) {
            fantasybit::OrderMeta om;
            readData(id.toStdString(),om);
            QString ret = om.DebugString().data();
            fantasybit::MerkleTree mt;
            readData(om.orderfillmetaid(),mt);
            for ( auto l : mt.leaves()) {
                QString a = l.data();
                ret += a.append(" {\n") + doGetDebugStr(l.data(),"OrderFillMeta").append("}\n");
            }
            return ret;
        }
        return "";
    }

    QString getExtra(const QString &nType, GOOGLE_NAMESPACE::protobuf::Message *msg) {
        auto t = msg->GetTypeName();

        if ( nType == "globalstateid" || nType == "GlobalStateMeta") {
            if ( t != "fantasybit.GlobalStateMeta")
                return t.append(" bad ").data();

            auto &tm = *(dynamic_cast<fantasybit::GlobalStateMeta *>(msg));
            return getExtra(tm);
        }

        if ( t == "fantasybit.MarketTicMeta" ) {
            auto &tm = *(dynamic_cast<fantasybit::MarketTicMeta *>(msg));
            return getExtra(tm);
        }
        //        if ( id == "market" ) {
        //            if ( t != "fantasybit.PlayerMarketState")
        //                return t.append(" bad ").data();

        //            auto &tm = *(dynamic_cast<fantasybit::PlayerMarketState *>(msg));
        //            return getExtra(id,tm);
        //        }


        return "";
    }

    QString getExtra(const fantasybit::GlobalStateMeta &msg) {
        return doGetDebugStr(msg.trmetaid().data(),"TrMeta");
    }

    QString getExtra(const fantasybit::MarketTicMeta &msg) {
        return doGetDebugStr(msg.orderfillhead().data(),"OrderFillMeta");
    }

    QString getExtra(const fantasybit::PlayerMarketState &msg) {
        return
                doGetDebugStr(msg.limitbookmetaid().data(),"limitbook").append (
                    doGetDebugStr(msg.marketticmetaid().data(),"markettic")
                    );
    }

    QString path() {
        return m_path;
    }

    void setPath(const QString in_path) {
        if ( m_path == in_path )
            return;
        m_path = in_path;
        init();
        emit pathChanged();
    }

    QMap<QString,GOOGLE_NAMESPACE::protobuf::Message *> typeMap;

    template <class T> void addThis()
    {
        T *inst = new T();
        typeMap.insert(T::descriptor()->name().data(),inst);
    }

    template <class T> void addThisName(QString in) {
        T *inst = new T();
        typeMap.insert(in,inst);
    }


    QString debugstring()
    {
        return m_dstr;
    }

    QStringList types()
    {
        return typeMap.keys()<< "string" << "market" << "limitbook" << "markettic" << "order" << "orders";
    }

    QString m_dstr;
    QString m_path;
    std::string write(const std::string &key, const std::string &val);
    std::string write(const std::string &val);
    std::string write(const GOOGLE_NAMESPACE::protobuf::Message &msg);
    std::string read(const std::string &id) ;
    std::string read(const std::string &id,
                     GOOGLE_NAMESPACE::protobuf::Message &msg);
signals:
    void pathChanged();
    void gotDebugStr();
signals:
    void error();

public slots:


private:
    fantasybit::BlockMeta bm;
    fantasybit::pbstate pbS;
    fantasybit::LeaderBoardMeta lbM;
    fantasybit::AwardMeta aM;
    fantasybit::ProjMeta projectionMeta;
    fantasybit::MerkleTree merkel;
};

#endif // LDBHASHREADERTOOL_H
