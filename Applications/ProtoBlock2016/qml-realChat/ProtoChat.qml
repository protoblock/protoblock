import QtQuick 2.1
import QtQuick.Controls 1.4
import Communi 3.0
import Material 1.0

Item {
    ChatPage {
        id: chatPage
        width: rootLoader.width
        height: rootLoader.height
        visible:true
        bufferModel: ircBufferModel
        serverBuffer: ircServerBuffer
    }
    function init(){
        chatPage.currentBuffer = ircServerBuffer
        ircConnectionPoint.sendCommand(cmd.createJoin("#protoblock"))
        ircConnectionPoint.open()
        pageHelper.title = "Protoblock Chat"
    }
    Component.onCompleted: {
        if(ircConnectionPoint.status !== IrcConnection.Connected){
               init()
        }else {

            console.log( "USER NAME  = " + ircConnectionPoint.displayName )
            chatPage.currentBuffer = ircServerBuffer
            ircBufferModel.connection = ircConnectionPoint
            ircBufferModel.add(ircServerBuffer)
            ircConnectionPoint.sendCommand(cmd.createJoin("#protoblock"))
            pageHelper.title = "Protoblock Chat"

        }
    }
    Irc { id: irc }
    IrcCommand { id: cmd }
    ProgressCircle {
       id: chatActInd
       anchors.centerIn: parent
       visible: false
   }
}
