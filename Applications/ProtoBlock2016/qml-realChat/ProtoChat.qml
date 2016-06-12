import QtQuick 2.1
import QtQuick.Controls 1.0
import Communi 3.0

Item {
    SystemPalette { id: palette }
    Irc { id: irc }
    IrcCommand { id: cmd }
    ChatPage {
        id: chatPage
        anchors.fill: parent
        visible:true
        bufferModel: IrcBufferModel {
            id: bufferModel
            sortMethod: Irc.SortByTitle
            connection: IrcConnection {
                id: connection
                host: "50.0.50.172"
                port: 6667
                secure: false
                saslMechanism: ""
                nickName: realRoot.uname
                realName:realRoot.uname
                userName:realRoot.uname
                password:""
            }
            onMessageIgnored: serverBuffer.receiveMessage(message)
            function quit() {
                bufferModel.clear()
                connection.quit("Fantasy Just Got Real")
                connection.close()
            }
        }
        serverBuffer: IrcBuffer {
            id: serverBuffer
            sticky: true
            persistent: true
            name: connection.displayName
            Component.onCompleted: bufferModel.add(serverBuffer)
        }
    }



    function init(){
        chatPage.currentBuffer = serverBuffer
        connection.sendCommand(cmd.createJoin("#protoblock"))
        connection.open()
        pageHelper.title = "Protoblock Chat"
    }
    Component.onCompleted:  init();
    Component.onDestruction: bufferModel.quit()
}
