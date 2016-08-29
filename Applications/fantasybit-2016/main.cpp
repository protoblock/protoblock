#include <QCoreApplication>
#include "core.h"
#include "PeerNode.h"
#include "FantasyAgent.h"

using namespace fantasybit;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Node node;

    node.init();
    bool ret = node.Sync();

    fantasybit::FantasyAgent agent{};
    fantasybit::NFLStateData data;
    fantasybit::FantasyNameData namedata;

    fantasybit::ExchangeData exchangedata;

    fantasybit::BlockProcessor processor(data,namedata, exchangedata);

    int32_t last_block = processor.init();

    auto gnum = node.getLastGlobalBlockNum();

    while ( last_block < *gnum) {
        int32_t next;
        next = last_block+1;
        auto b = fantasybit::Node::getLocalBlock(next);
        if (!b) {
            qWarning() << " !b";
            break;
            //        return false;
        }
        else {

            int32_t last = processor.process(*b);
            if ( last == -1 ) {
                qDebug() << " last = -1";
                break;

                //emit OnError();
    //            timer->start(5000);
    //            return false;
            }


            else {
    //            std::lock_guard<std::recursive_mutex> lockg{ last_mutex };
                if ( last == last_block+1)
                    last_block = last;
                else {
                    qDebug() << " shoud bever be here! ? reorg? fork? ";
                    break;
                }
            }
        }
    }

    return a.exec();
}
