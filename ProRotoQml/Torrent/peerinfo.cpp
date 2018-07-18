#include "peerinfo.h"

void PeerInfo::clear()
{
    m_Address.clear();
    m_Port = 0;
    m_Id.clear();
}
