import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import Material 1.0
import Material.ListItems 1.0 as ListItems
import "qwebchannel.js" as WebChannel

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Utils 1.0

Item {
    id: chatRoot
    property var channel;
    Component.onCompleted: {
        pageHelper.title = "Chat"
        socket.active = true;
        runner.start()
    }

    Card{
        id: chatElements
        anchors.fill: parent
        anchors.margins: ProtoScreen.guToPx(1.25)
        elevation: 5
        Grid {
            id: grid
            columns: 2
            width: chatElements.width
            height: chatElements.height
            anchors.margins: ProtoScreen.guToPx(2.5)
            TextArea {
                id: chat
                text: ""
                width: chatElements.width - userlist.width
                height: chatElements.height - messageBox.height
                textFormat: TextEdit.RichText
                readOnly: true
            }

            ListView{
                id: userlist
                width: ProtoScreen.guToPx(18.75)
                height: chatElements.height - messageBox.height
                header: ListItems.Standard{backgroundColor: "#EFF6F9"; text: "Users"; elevation: 2}
                model: usersModel
                clip: true
                delegate:ListItems.Standard{
                    elevation: 1
                    text: model.name
                }
            }
            TextField {
                id: messageBox
                helperText: "Enter some text to talk with others "
                width: parent.width
                height:  ProtoScreen.guToPx(5)
                onEditingFinished: {
                    if (messageBox.text.length)
                        //call the sendMessage method to send the message
                        chatRoot.channel.objects.chatserver.sendMessage(realRoot.uname, messageBox.text);
                    messageBox.text = '';
                }
            }
        }
    }





    ProtoblockSocket {
        id: socket
        url: MiddleMan.chatServerAddr;
        active: false
        property var send: function (arg) {
            sendTextMessage(arg);
        }
        onTextMessageReceived: {
            onmessage({data: message});
        }
        property var onmessage;
        onStatusChanged: {
            if (socket.status == ProtoblockSocket.Error) {
                console.error("Error: " + socket.errorString)
                chatTimer.start();
            }
            else if (socket.status == ProtoblockSocket.Closed) {
                chatTimer.start();
                //                messageBox.app += "\nSocket closed"
            }
            else if (socket.status == ProtoblockSocket.Open) {
                chatTimmer.stop()
                //open the webchannel with the socket as transport
                new WebChannel.QWebChannel(socket, function(ch) {
                    chatRoot.channel = ch;
                    //connect to the changed signal of the userList property
                    ch.objects.chatserver.userListChanged.connect(function(args) {
                        usersModel.clear();
                        ch.objects.chatserver.userList.forEach(function(user) {
                            usersModel.append({"name" :  user})
                        });
                    });
                    //connect to the newMessage signal
                    ch.objects.chatserver.newMessage.connect(function(time, user, message) {
                        var msg = "[" + time + "] <span style='color:red' >" + user + "</span> : " +  message
                        chat.append( msg);
                    });
                    //connect to the keep alive signal
                    ch.objects.chatserver.keepAlive.connect(function(args) {
                        if (realRoot.uname !== '')
                            //and call the keep alive response method as an answer
                            ch.objects.chatserver.keepAliveResponse(realRoot.uname)
                    });

                    //                    ch.objects.chatserver.

                });
            }
        }
    }

    /*!
  run the timmer once every two seconds to reconnect to the server
  */
    Timer{
        id: chatTimmer
        interval: 2000
        running: false
        repeat: true
        onTriggered: {
            if (socket.status !== ProtoblockSocket.Open){
                socket.active = false
                socket.active = true
            }
        }
    }
    ListModel{id: usersModel}
    Timer{
        id: runner
        interval: 1200
        repeat: false
        running: false
        onTriggered: {
            if (socket.status === ProtoblockSocket.Open){
                console.log("getting ready to login " +  realRoot.uname)
                login();
            }else{
                runner.start()
            }
        }
    }

    Timer{
        id: recconectionTimmer
        interval: 1200
        repeat: NumberAnimation.Infinite
        running: false
        onTriggered: {
            if (socket.status !== WebSocket.Open){
                reconnect()
            }else{
                recconectionTimmer.stop
            }
        }
    }

    function reconnect(){
        socket.active = false
        socket.active = true
    }

    function login(){
        chatRoot.channel.objects.chatserver.login(realRoot.uname, function(arg) {
            //check the return value for success
            if (arg === true) {
                console.log("logged in as " + realRoot.uname)
            } else {
            }
        });
    }
}