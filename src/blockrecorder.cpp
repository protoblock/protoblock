#include <blockrecorder.h>
#include <QDebug>
#include <leveldb/db.h>

using namespace fantasybit ;

void BlockRecorder::init() {
    write_sync.sync = true;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;
    status = leveldb::DB::Open(options, filedir("blockstatus"), &blockstatus);

    std::string value;
    status = blockstatus->Get(leveldb::ReadOptions(), "lastblock", &value);
    if (!status.ok()) {
        lastBlock =  0;
        qWarning() << "!ok no blocks";
    }
    else {
        lastBlock = *(reinterpret_cast<const int *>(value.data()));
        qInfo() << "lastBLock: " << lastBlock;
    }
}

/*
void BlockRecorder::initAllData() {
    initFantasyNames();
    initProjections();
    initData();
}

void BlockRecorder::initFantasyNames() {
    auto *it = namehashpub->NewIterator(leveldb::ReadOptions());
    std::string fname;
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (pubfantasyname->Get(leveldb::ReadOptions(), it->value(), &fname)
                .IsNotFound()) {
            qWarning() << it->value().ToString()
                            << " found namehash but NO fantasyname ";
            continue;
        }

        auto pfn = Commissioner::AddName(fname, it->value().ToString());
        uint64_t newval = 0;
        std::string temp;
        if (pubbalance->Get(leveldb::ReadOptions(), it->value(), &temp).IsNotFound())
        {
            qWarning() << "nno bal " << pfn->ToString();
        }
        else {

            newval = *(reinterpret_cast<const uint64_t *>(temp.data()));
            pfn->addBalance(newval);
        }
        qDebug() << pfn->ToString();

    }
    delete it;
}

void BlockRecorder::initProjections() {
    auto *it = projections->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {

        auto str = it->key().ToString();
        int pos =  str.find_first_of(':');
        auto nflplayer = str.substr(0, pos);
        auto fantasyname = str.substr(pos + 1);

        uint64_t bal = *(reinterpret_cast<const uint64_t *>(it->value().data()));

        FantasyProjections::Projections[nflplayer][fantasyname] = bal;
        qDebug() << str << ":" << bal;
    }
    delete it;
}

void BlockRecorder::initData() {
    PlayerData pd{};
    auto it = players->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        pd.ParseFromString(it->value().ToString());
        (Source::TeamPlayers[pd.teamid()]).insert(pd.playerid());
        Source::PlayerTeam.emplace(pd.playerid(), pd.teamid());
        qDebug() << "Player:" << pd.playerid() << ":" << pd.teamid();
    }

    TeamData td{};
    it = teams->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        td.ParseFromString(it->value().ToString());
        Source::TeamIds.insert(td.teamid());
        qDebug() << td.teamid();
    }

    TeamState ts{};
    it = state->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (it->key().ToString() == "globalstate")
            continue;
        ts.ParseFromString(it->value().ToString());
        Source::TeamWeek[ts.teamid()] =
                std::make_pair(ts.week(), ts.state() == TeamState::INGAME);
        qDebug() << ts.teamid() << " " << ts.state() << " week" << ts.week();
    }
}

DeltaData BlockRecorder::DeltaSnap() {
    DeltaData dd{};

    FantasyPlayer fp{};

    for (auto pp : Commissioner::GetFantasyNames()) {
        qDebug() << pp->ToString();
        fp.set_name(pp->alias());
        fp.set_bits(pp->getBalance());
        auto *p = dd.add_players();
        p->CopyFrom(fp);
    }

    dd.mutable_globalstate()->CopyFrom(GetGlobalState());

    TeamState ts{};
    auto it = state->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (it->key().ToString() == "globalstate")
            continue;
        ts.ParseFromString(it->value().ToString());
        auto *p = dd.add_teamstates();
        p->CopyFrom(ts);
    }

    //std::unordered_map<std::string, std::string> PlayerTeam{};
    //std::map<std::string, std::unordered_set<std::string>> TeamPlayers{};

    Data d{};
    PlayerData pd{};
    it = players->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        pd.ParseFromString(it->value().ToString());
        d.set_type(Data_Type::Data_Type_PLAYER);
        d.MutableExtension(PlayerData::player_data)->CopyFrom(pd);
        auto *p = dd.add_datas();
        p->CopyFrom(d);
        (Source::TeamPlayers[pd.teamid()]).insert(pd.playerid());
        Source::PlayerTeam.emplace(pd.playerid(), pd.teamid());
    }

    d.Clear();
    TeamData td{};
    it = teams->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        td.ParseFromString(it->value().ToString());
        d.set_type(Data_Type::Data_Type_TEAM);
        d.MutableExtension(TeamData::team_data)->CopyFrom(td);
        auto *p = dd.add_datas();
        p->CopyFrom(d);
        Source::TeamIds.insert(td.teamid());
    }


    return dd;
}
*/

/*
void BlockRecorder::OnGlobalState(const GlobalState &gs) {
    state->Put(leveldb::WriteOptions(), "globalstate", gs.SerializeAsString());
    qDebug() << gs.DebugString();
}

GlobalState BlockRecorder::GetGlobalState() {
    GlobalState gs{};
    std::string temp;
    if (state->Get(leveldb::ReadOptions(), "globalstate", &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else {
        qCritical() << "No GlobalState";
    }

    return gs;
}

void BlockRecorder::OnTeamState(const TeamState &gs) {
    state->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
    Source::TeamWeek[gs.teamid()] = std::make_pair(gs.week(), gs.state() == TeamState::INGAME);
    qDebug() << gs.DebugString();
}

TeamState BlockRecorder::GetTeamState(const std::string &teamid) {
    TeamState gs{};
    std::string temp;
    if (state->Get(leveldb::ReadOptions(),teamid, &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else {
        qCritical() << teamid << ": No TeamState";
    }

    return gs;
}

void BlockRecorder::OnPlayerData(const PlayerData &gs) {
    players->Put(leveldb::WriteOptions(), gs.playerid(), gs.SerializeAsString());
    Source::PlayerTeam[gs.playerid()] = gs.teamid();
    (Source::TeamPlayers[gs.teamid()]).insert(gs.playerid());
    qDebug() << gs.DebugString();
}

PlayerData BlockRecorder::GetPlayerData(const std::string &pid) {
    PlayerData gs{};
    std::string temp;
    if (players->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else qCritical() << pid << " no PlayerData";

    return gs;
}

void BlockRecorder::OnTeamData(const TeamData &gs) {
    teams->Put(leveldb::WriteOptions(), gs.teamid(), gs.SerializeAsString());
    Source::TeamIds.insert(gs.teamid());
    qDebug() << gs.DebugString();
}

TeamData BlockRecorder::GetTeamData(const std::string &pid) {
    TeamData gs{};
    std::string temp;
    if (teams->Get(leveldb::ReadOptions(), pid, &temp).ok()) {
        gs.ParseFromString(temp);
    }
    else qCritical() << pid << ": no TeamData";

    return gs;
}
*/
void BlockRecorder::startBlock(int num) {
    leveldb::Slice value((char*)&num, sizeof(int));
    blockstatus->Put(write_sync, "processing", value);
    blockstatus->Put(leveldb::WriteOptions(), "lastblock", value);
    qInfo() << "starting block: " << num;
}


int BlockRecorder::endBlock(int num) {
    int none = -1;
    leveldb::Slice value((char*)&none, sizeof(int));
    blockstatus->Put(write_sync, "processing", value);
    qInfo() << "end block: " << num;
    return num;
}


bool BlockRecorder::isValid() {
    std::string value;
    if (blockstatus->Get(leveldb::ReadOptions(), "processing", &value).IsNotFound())
        return true;

    int num = *(reinterpret_cast<const int *>(value.data()));
    return num < 0;
}


int BlockRecorder::getLastBlockId() {
    return lastBlock;
}

/*
void BlockRecorder::recordName(const hash_t &hash,const std::string &pubkey,const std::string &name)
{
    leveldb::Slice hkey((char*)&hash, sizeof(hash_t));
    namehashpub->Put(leveldb::WriteOptions(), hkey, pubkey);
    pubfantasyname->Put(leveldb::WriteOptions(), pubkey, name);

    std::string temp;
    if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).IsNotFound())
    {
        uint64_t bal = 0;
        leveldb::Slice bval((char*)&bal, sizeof(uint64_t));
        pubbalance->Put(leveldb::WriteOptions(), pubkey, bval);
    }

    qDebug() << name << ":" <<  hash << ":" << pubkey;
}

void BlockRecorder::addProjection(const std::string &fname, const FantasyPlayerPoints &fpp)
{
    uint64_t bal = fpp.points();
    leveldb::Slice bval((char*)&bal, sizeof(uint64_t));

    std::string key{ fpp.playerid() };
    std::string fn{ fname };

    projections->Put(leveldb::WriteOptions(), key.append(":").append(fname), bval);
    auto iter = FantasyProjections::Projections.find(fpp.playerid());
    if (iter == end(FantasyProjections::Projections)) {
        //FantasyProjections::Projections.insert(key, { fn, bal });
        FantasyProjections::Projections[fpp.playerid()][fn] = bal;
    }
    else
        iter->second[fname] = bal;

    qDebug() << "proj: " << key << ":" << fname << ":" << bal;
}

void BlockRecorder::clearProjections() {
    auto *it = projections->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
        projections->Delete(leveldb::WriteOptions(), it->key());

    FantasyProjections::Projections.clear();
    qDebug() << " clearProjections ";
}

void BlockRecorder::addBalance(std::string &pubkey,uint64_t add)
{
    uint64_t curr = 0;
    std::string temp;

    if (pubbalance->Get(leveldb::ReadOptions(), pubkey, &temp).ok())
    {
        curr = *(reinterpret_cast<const uint64_t *>(temp.data()));
    }

    qDebug() << pubkey << " bal " << curr << "+" << add;

    uint64_t newval = curr + add;
    leveldb::Slice bval((char*)&newval, sizeof(uint64_t));
    pubbalance->Put(leveldb::WriteOptions(), pubkey, bval);

    auto it = Commissioner::getName(Commissioner::str2pk(pubkey));

    if (it != nullptr)
        it->addBalance(add);
    else
        qCritical() << " cant find FantasyName";
}
*/
std::string BlockRecorder::filedir(const std::string &in) {
    return GET_ROOT_DIR() + "index/" + in;
}

void BlockRecorder::closeAll() {
    delete blockstatus;
}


