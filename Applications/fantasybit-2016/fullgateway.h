#ifndef FULLGATEWAY_H
#define FULLGATEWAY_H

#include "pbgateways.h"

namespace pb {

class FullGateway : public QObject, public IPBGateway, public IDataService {
    Q_OBJECT
    Q_INTERFACES(pb::IPBGateway pb::IDataService)
public:
    FullGateway();
};

}
#endif // FULLGATEWAY_H
