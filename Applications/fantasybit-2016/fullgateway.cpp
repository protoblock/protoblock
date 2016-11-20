#include "fullgateway.h"

namespace  pb {

void FullGateway::OnMarketSnapShot(MarketSnapshot *mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "FullGateway OnMarketSnapShot " << mt->DebugString().data();
#endif

    m_PlayerQuoteSliceModel.Update(mt);
}


void FullGateway::OnMarketTicker(fantasybit::MarketTicker *mt) {
    if ( mt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "FullGateway OnMarketTicker " << mt->DebugString().data();
#endif

    m_PlayerQuoteSliceModel.Update(mt);

}

void FullGateway::OnTradeTick(fantasybit::TradeTic* tt) {
    if ( tt->symbol() == "" )
        return;
#ifdef TRACE
    qDebug() << "FullGateway TradeTic " << tt->DebugString().data();
#endif

    m_PlayerQuoteSliceModel.Update(tt);
}

void FullGateway::OnDepthDelta(fantasybit::DepthFeedDelta*) {}
void FullGateway::OnMyNewOrder(fantasybit::Order& ord) {}
void FullGateway::OnNewPos(fantasybit::FullPosition) {}
void FullGateway::OnNewOO(fantasybit::FullOrderDelta) {}

}
