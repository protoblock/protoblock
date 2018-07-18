#include "peertwopeerchat_plugin.h"
#include "peertwopeer.h"
#include "chatdialog.h"

#include <qqml.h>

void PeerTwoPeerChatPlugin::registerTypes(const char *uri)
{
    // @uri PeerTwoPeer
    qmlRegisterType<PeerTwoPeer>(uri, 1, 0, "PeerTwoPeer");
    qmlRegisterType<ChatDialog>(uri,1,0,"Chat");

}

