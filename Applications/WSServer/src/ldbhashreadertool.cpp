#include "ldbhashreadertool.h"
#include "StateData.pb.h"
#include <QCryptographicHash>

#include <QMap>
using std::string;

LdbHashReaderTool::LdbHashReaderTool(QObject *parent) {
    LdbHashReaderTool::addThis<fantasybit::BlockMeta>();
    LdbHashReaderTool::addThis<fantasybit::TxMeta>();
    LdbHashReaderTool::addThis<fantasybit::TrMeta>();
    LdbHashReaderTool::addThis<fantasybit::GameMeta>();
    LdbHashReaderTool::addThis<fantasybit::DataMeta>();
    LdbHashReaderTool::addThis<fantasybit::LeaderBoardMeta>();
    LdbHashReaderTool::addThis<fantasybit::FantasyNameBalMeta>();
    LdbHashReaderTool::addThis<fantasybit::FantasyNameState>();
    LdbHashReaderTool::addThis<fantasybit::WeekGameStatusMeta>();
    LdbHashReaderTool::addThis<fantasybit::InGameProjMeta>();
    LdbHashReaderTool::addThis<fantasybit::GameResultsMeta>();
    LdbHashReaderTool::addThis<fantasybit::GameStatusMeta>();
    LdbHashReaderTool::addThis<fantasybit::TeamProjMeta>();
    LdbHashReaderTool::addThis<fantasybit::TeamResultMeta>();
    LdbHashReaderTool::addThis<fantasybit::GamePlayerProjMeta>();
    LdbHashReaderTool::addThis<fantasybit::ProjMeta>();
    LdbHashReaderTool::addThis<fantasybit::PosMeta>();
    LdbHashReaderTool::addThis<fantasybit::PlayerResultMeta>();
    LdbHashReaderTool::addThis<fantasybit::AwardMeta>();
    LdbHashReaderTool::addThis<fantasybit::PnlMeta>();
    LdbHashReaderTool::addThis<fantasybit::PlayerMarketState>();
    LdbHashReaderTool::addThis<fantasybit::LimitBookMeta>();

    LdbHashReaderTool::addThis<fantasybit::MarketTicMeta>();
    LdbHashReaderTool::addThis<fantasybit::OrderMeta>();
    LdbHashReaderTool::addThis<fantasybit::OrderFillMeta>();
    LdbHashReaderTool::addThis<fantasybit::GlobalStateMeta>();

    LdbHashReaderTool::addThis<fantasybit::PlayerMeta>();
    LdbHashReaderTool::addThis<fantasybit::TeamMeta>();
    LdbHashReaderTool::addThis<fantasybit::pbstate>();
    LdbHashReaderTool::addThis<fantasybit::MerkleTree>();
    LdbHashReaderTool::addThis<fantasybit::PlayerGameStats>();
    LdbHashReaderTool::addThisName<fantasybit::ProjMeta>("projmetaid");


    m_path = "D:/data/block/meta";
    init();
}

QStringList LdbHashReaderTool::getChild(const QString nodesParent)
{

    LdbHashReaderMap lMapTool;

//    qDebug() << lMapTool.getKey (nodesParent);


    QString pbState = jumpToPbStateId ();
    getDebugStr (pbState, "pbstate");


    QStringList valOne = m_dstr.split(QRegExp("\n|\r\n|\r"));
//    qDebug() << "VAL ONE \t"<<valOne;
    QStringList valTwo, valThree, theHashes;


    foreach (QString li, valOne) {
        if (li.contains ("{") || li.contains ("}") )
        {
//            qDebug() << "there is a { or a } ";
        }
        else
        {
            valTwo = li.split (QRegExp(":"));
            //                    valTwo.pop_back ();

            if (valTwo.length () > 1){
                valThree << valTwo.at (0);
                theHashes << valTwo.at (1);
            }
        }
    }


    //qDebug() << "val three " << valThree;
    QStringList valFour ;
    QString fullGuess;
//    qDebug() << "val Length "  << valThree.length ();
    for(int i = 0 ; i< valThree.length(); i++)
    {
        fullGuess =  QString("%1 : %2").arg (valThree.at(i)).arg (lMapTool.getKey (valThree.at (i)) );
        valFour << fullGuess;
    }

    QMap<QString , QPair<QString,QString>> theFinalGuess;
    for (int ii = 0 ; ii < theHashes.length (); ii++ )
    {
        QString sdf = theHashes.at (ii);
        sdf.remove (QRegExp("\""));
        sdf.remove (QRegExp(" "));
//        qDebug() << "FORMAP " << sdf << valFour.at (ii) <<"\n";
    }

    return valFour;
}

string LdbHashReaderTool::write(const string &key, const string &val) {
    //qDebug() << key.data() << val.data();
    db->Put(write_sync,key,val);
    return key;
}

string LdbHashReaderTool::write(const string &val) {
    QString valToQString  = QString::fromStdString (val);
    QByteArray results = QCryptographicHash::hash( valToQString.toLatin1() , QCryptographicHash::Sha256 );
    QString hashResult(results.toHex());

    /// back to the std::string
    auto ret = hashResult.toStdString ();

    write(ret,val);
    return ret;

    // auto ret = fc::sha256::hash(val).str();
//    write(ret,v//al);
//    return ret;
}

string LdbHashReaderTool::write(const GOOGLE_NAMESPACE::protobuf::Message &msg) {
    return write(msg.SerializeAsString());
}

std::string LdbHashReaderTool::read(const std::string &id) {
    string value;
    db->Get(leveldb::ReadOptions(),id,&value);
    return value;
}

string LdbHashReaderTool::read(const string &id, GOOGLE_NAMESPACE::protobuf::Message &msg) {
    auto str = read(id);
    msg.ParseFromString(str);
    return str;
}

LdbHashReaderTool *LdbHashReaderTool::myInstance;

