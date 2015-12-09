//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 8/24/14.
//
//

#ifndef __fantasybit__Processor__
#define __fantasybit__Processor__

#include <QObject>

#include "blockrecorder.h"
#include "Data.h"
#include "NameData.h"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include "ProtoData.pb.h"
#include "ApiData.pb.h"
#include "timestate.h"
#include "ExchangeData.h"

namespace fantasybit
{

class BlockProcessor : public QObject {

    Q_OBJECT


    BlockRecorder mRecorder{};
    NFLStateData &mData;
    FantasyNameData &mNameData;
    ExchangeData &mExchangeData;
    int32_t realHeight = 0;
    int32_t lastidprocessed = 0;
    //GlobalState mGlobalState{};
    bool verify_name(const SignedTransaction &, const NameTrans &,
                     const fc::ecc::signature&, const fc::sha256 &);
    bool amlive = false;
public slots:
    void OnLive(bool) {
        amlive = true;
    }

signals:
    void WeekStart(int);
    void WeekOver(int);
    void InvalidState(int);   
    void new_dataDistribution(fantasybit::Distribution);
    void onControlMessage(QString);


public:
    BlockProcessor(NFLStateData &data, FantasyNameData &namedata,
                   ExchangeData &ed) :
        mData(data), mNameData(namedata) , mExchangeData(ed) {}

    int32_t init();

    int32_t process(Block &sblock);
    bool processDataBlock(const Block &sblock);
    //bool isInWeekGame(const std::string &id, int week );
    //bool sanity(const FantasyPlayerPoints &fpp);
	void process(decltype(DataTransition::default_instance().data()) in, 
                const std::string &blocksigner );
    void process(const DataTransition &indt);
    bool isValidTx(const SignedTransaction &st);
    void processTxfrom(const Block &b,int start = 0);
    static bool verifySignedBlock(const Block &sblock);
    static bool verifySignedTransaction(const SignedTransaction &st);
    static std::shared_ptr<FantasyName>
            getFNverifySt(const SignedTransaction &st);
    void processResultProj(PlayerResult* playerresult,
                           std::unordered_map<std::string,int> &proj,
                           BookPos *,
                           const std::string &blocksigner);

    void OnWeekOver(int week);
    void OnWeekStart(int week);

    void hardReset();
    void ProcessInsideStamped(const SignedTransaction &inst, int32_t);


    static double CalcResults(const Stats &stats) {
        int ret = 0;
        double iret = 0;

        if ( stats.has_ostats() ) {
            auto os = stats.ostats();
            if ( os.has_passtd())
                ret += 400 * os.passtd(); //PAssing Yards (QB)


            if ( os.has_rushtd() )
                ret += 600 * os.rushtd();

            if ( os.has_rectd() )
                ret += 600 *  os.rectd();

            if ( os.has_passyds() )
               ret += 5 *  os.passyds();

            if ( os.has_recyds() )
                ret += 10 *  os.recyds();

            if (  os.has_rushyds() )
                ret += 10 *  os.rushyds();

            if ( os.has_rec() )
                ret += 100 * os.rec();

            if ( os.has_pint() )
                ret += -100 * os.pint();

            if ( os.has_twopt() )
                ret += 200 * os.twopt();

            if (os.has_onept())
                ret += 200 *  os.onept();
        }
        if ( stats.has_kstats() ) {
            auto ks = stats.kstats();
            if ( ks.has_pa() )
                ret += 100 * ks.pa();
            for ( auto f : ks.fg())
                ret += 300 + 10 * ((f > 30) ? (f-30) : 0);
        }
        if ( stats.has_dstats() ) {
            auto ds = stats.dstats();
            if ( ds.has_deftd())
                ret += 600 * ds.deftd();
            if ( ds.has_onept())
                ret += 200 * ds.onept();
            if ( ds.has_ptsa()) {
                if ( ds.ptsa() == 0)
                    ret += 1200;
                else if ( ds.ptsa() < 7)
                    ret += 100;
                else if ( ds.ptsa() < 11)
                    ret += 800;
            }
            if ( ds.has_sacks())
                ret += 100 * ds.sacks();
            if ( ds.has_sfty())
                ret += 500 * ds.sfty();
            if ( ds.has_turnovers())
                ret += 200 * ds.turnovers();
            if ( ds.has_twopt())
                ret += 200 * ds.twopt();
        }

        if ( ret == 0) return 0.0;

        iret = (double)ret / 100.0;
        return iret;
    }

};

}

#endif

