//
//  Comissioner.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#include <unordered_map>
#include <memory>

#include "Commissioner.h"
//#include <iostream>
//#include <ctime>
#include "FantasyName.h"
//#include "DataPersist.h"
//#include "platform.h"
//#include "appsettings.h"
//#include "ldbwriter.h"
//#include "qurl.h"
//#include "RestFullCall.h"
//#include "pbutils.h"

using namespace std;

namespace fantasybit {

pb::public_key_data Commissioner::GENESIS_PUB_KEY
{
#ifndef PRODFOOTBALL
      Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"))
#else
      Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
#endif
};

#ifndef PRODFOOTBALL
pb::public_key_data Commissioner::PROD_GENESIS_PUB_KEY
{
      Commissioner::str2pk(std::string("25dTUQHwaPHdN2fXjpryz5jrrXxU6NNfKgrpJRA4VheJ4"))
};
#endif

//std::unordered_map<pb::public_key_data,std::shared_ptr<FantasyName>> Commissioner::FantasyNames{};
//std::map<hash_t,pb::public_key_data> Commissioner::Hash2Pk{};

//std::recursive_mutex Commissioner::name_mutex{};

}
