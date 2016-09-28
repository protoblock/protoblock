#ifndef IMPORTLOADER
#define IMPORTLOADER

#include <QFile>
#include "ProtoData.pb.h"
#include "fbutils.h"
#include <QObject>
#include <unordered_set>
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlConstRefPropertyHelpers.h"
#include "dataservice.h"
#include "mediator.h"
namespace pb  {
using namespace  std;
using namespace fantasybit;


class ImportLoader : public QObject {
    Q_OBJECT

//    QML_WRITABLE_AUTO_PROPERTY(QString, filename)
    QML_READONLY_CSTREF_PROPERTY (QString, filename)

public:
    static unordered_set<string> headers;
    unordered_map<string,int> headermap;
    bool mDirect;
    QString mFilename;
    explicit ImportLoader(QObject *parent = 0) : QObject(parent) {
        string usefilename = GET_ROOT_DIR() + "ProtoblockImportExportProjections.csv";

        m_filename = usefilename.data();
    }
    int playeridindex = -1;
    int projectionindex = -1;

    Q_INVOKABLE QString doImport(QString fantasyname) {
        Mediator::instance()->CopyTheseProjections(loadProjFromFile());
        return m_filename;
    }

    std::vector<fantasybit::PlayerPoints> loadProjFromFile() { //string infile = "ProtoblockImportLoader.csv") {

        std::vector<fantasybit::PlayerPoints> result;
//        string filename = GET_ROOT_DIR() + infile;

        //qDebug() << " ccccc " << filename;
        QFile file(m_filename);
        QTextStream in(&file);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QStringList headerdata,listdata;
            QString line;

            line = in.readLine();
            headerdata.append(line.split(','));
            for ( int i = 0;i<headerdata.length();i++) {
                string head = headerdata.at(i).toStdString();
//                if ( headers.find(head) != end(headers))
//                    headermap[head] = i;
                if ( head == "ProtoblockPlayerID")
                    playeridindex = i;
                else if ( head == "MyProjection")
                    projectionindex = i;
            }
            if ( playeridindex == -1 && projectionindex != 0)
                playeridindex = 0;
            if ( projectionindex == -1 && playeridindex != 1)
                playeridindex = 1;

            if ( projectionindex == -1 || playeridindex == -1 )
                return result;

            while (!in.atEnd()) {
                listdata.clear();
                line = in.readLine();
                //qDebug() << line;
                listdata.append(line.split(','));
                QString errorParsingObject;

                PlayerPoints pp = getProjFromList(listdata,errorParsingObject);
                if ( errorParsingObject != "" ) continue;
                result.push_back(pp);

            }
            qDebug() << "out loop" << file.atEnd();

        }

        return result;
    }

    Q_INVOKABLE QString doExport(QString fantasyname) {
//        string usefilename = GET_ROOT_DIR() + m_filename.toStdString();
//        QString retfilename = usefilename.data();
        QFile file(m_filename);
        QTextStream out(&file);
        auto inrosters = DataService::instance()->GetCurrentWeekGameRosters();
        auto  recentProjections = DataService::instance()->GetProjByName(fantasyname.toStdString());

        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly ))
            return "error";

        {
            out << "PlayerName" << ","
                << "Position" << ","
                << "Team" << ","
                   << "ProtoblockPlayerID" << ","
                      << "MyProjection"
                      << "\n";
        }
        for(const pb::GameRoster & game  : inrosters) {
            //add home players
            for(const auto& player : game.homeroster) {
                out     << (player.second.base.first() + " " + player.second.base.last() ).data() << ","
                        << player.second.base.position().data() << ","
                        << game.info.home().data() << ","
                        << player.first.data() << ","
                        << to_string(recentProjections[player.first]).data()
                        << "\n";
            }

            //add away players
            for(const auto& player : game.awayroster) {
                out     << (player.second.base.first() + " " + player.second.base.last() ).data() << ","
                        << player.second.base.position().data() << ","
                        << game.info.away().data() << ","
                        << player.first.data() << ","
                        << to_string(recentProjections[player.first]).data()
                        << "\n";
            }
        }

        return m_filename;
    }


    fantasybit::PlayerPoints getProjFromList(QStringList & list,QString & errorParsingObject) {
       PlayerPoints pp{};

       if ( list.length()-1 < playeridindex || list.length()-1 < projectionindex)
           errorParsingObject = " -1";
       else {
           errorParsingObject ="";
            pp.set_playerid(list.at(playeridindex).toStdString());
            pp.set_points(stoi(list.at(projectionindex).toStdString()));
       }
       return pp;
    }
/*
    void getStats(ProjStats *stat,QStringList & list) const {

       ProjOstats os{};
       float i = 0.0;
       i = list[headers["pyds"]].toFloat();
       os.set_passyds(i);
       i = list[headers["ruyds"]].toFloat();
       os.set_rushyds(i);
       i = list[headers["reyds"]].toFloat();
       os.set_recyds(i);
       i = list[headers["ptd"]].toFloat();
       os.set_passtd(i);
       i = list[headers["rutd"]].toFloat();
       os.set_rushtd(i);
       i = list[headers["retd"]].toFloat();
       os.set_rectd(i);
       i = list[headers["rec"]].toFloat();
       os.set_rec(i);
       i = list[headers["pint"]].toFloat();
       os.set_pint(i);
       i = list[headers["fumble"]].toFloat();
       os.set_fumble(i);
       stat->mutable_ostats()->CopyFrom(os);

       ProjDstats ds{};
       i = list[headers["sack"]].toFloat();
       ds.set_sacks(i);
       i = list[headers["sfty"]].toFloat();
       ds.set_sfty(i);
       i = list[headers["int"]].toFloat();
       i += list[headers["fumble"]].toFloat();
       ds.set_turnovers(i);
       i = list[headers["dtd"]].toFloat();
       ds.set_deftd(i);
       i = list[headers["pa0"]].toFloat();
       if ( i == 1)
           ds.set_ptsa(0.1);
       else {
           i = list[headers["pa6"]].toFloat();
           if ( i == 1)
               ds.set_ptsa(6);
           else {
               i = list[headers["pa13"]].toFloat();
               if ( i == 1)
                   ds.set_ptsa(13);

           }

       }

       stat->mutable_dstats()->CopyFrom(ds);

       ProjKstats ks{};
       i = list[headers["xp"]].toFloat();
       ks.set_pa(i);
       i = list[headers["fg19"]].toFloat();
       ks.set_fg30(i);
       i = list[headers["fg29"]].toFloat();
       ks.set_fg30(i + ks.fg30());
       i = list[headers["fg39"]].toFloat();
       ks.set_fg40(i);
       i = list[headers["fg49"]].toFloat();
       ks.set_fg50(i);
       i = list[headers["fg50p"]].toFloat();
       ks.set_fg60(i);

       stat->mutable_kstats()->CopyFrom(ks);

    }

    static double CalcProj(const ProjStats &stats) {
        double ret = 0;

        if ( stats.has_ostats() ) {
            auto os = stats.ostats();
            if ( os.has_passtd())
                ret += 4.0 * os.passtd(); //PAssing Yards (QB)


            if ( os.has_rushtd() )
                ret += 6.0 * os.rushtd();

            if ( os.has_rectd() )
                ret += 6.0 *  os.rectd();

            if ( os.has_passyds() )
               ret += .05 *  os.passyds();

            if ( os.has_recyds() )
                ret += .1 *  os.recyds();

            if (  os.has_rushyds() )
                ret += .1 *  os.rushyds();

            if ( os.has_rec() )
                ret += 1.0 * os.rec();

            if ( os.has_pint() )
                ret += -1.0 * os.pint();

            if ( os.has_twopt() )
                ret += 2.0 * os.twopt();

            if (os.has_onept())
                ret += 2.0 *  os.onept();
        }
        if ( stats.has_kstats() ) {
            auto ks = stats.kstats();
            if ( ks.has_pa() )
                ret += 1.0 * ks.pa();
            if ( ks.has_fg30() )
                ret += 3.0 * ks.fg30();
            if ( ks.has_fg40() )
                ret += 4.0 * ks.fg40();
            if ( ks.has_fg50() )
                ret += 5.0 * ks.fg50();
            if ( ks.has_fg60() )
                ret += 6.0 * ks.fg60();
        }

        if ( stats.has_dstats() ) {
            auto ds = stats.dstats();
            if ( ds.has_deftd())
                ret += 6.0 * ds.deftd();
            if ( ds.has_onept())
                ret += 2.0 * ds.onept();
            if ( ds.has_ptsa() && ds.ptsa() > 0.0) {
                if ( ds.ptsa() < 1.0)
                    ret += 12.0;
                else if ( ds.ptsa() < 7.0)
                    ret += 10.0;
                else if ( ds.ptsa() < 11.0)
                    ret += 8.0;
                else if ( ds.ptsa() < 14.0)
                    ret += 4.0;

            }
            if ( ds.has_sacks())
                ret += 1.0 * ds.sacks();
            if ( ds.has_sfty())
                ret += 5.0 * ds.sfty();
            if ( ds.has_turnovers())
                ret += 2.0 * ds.turnovers();
            if ( ds.has_twopt())
                ret += 2.0 * ds.twopt();
        }

        return ret;
    }
*/

};

}
#endif

