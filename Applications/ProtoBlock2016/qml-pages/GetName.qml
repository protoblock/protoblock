import QtQuick 2.0
import QtQuick.Controls 1.4 as Contols
import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

Item {
    id: loginPage
    property string defaultTxt: "Please Register to play Protoblock" //"A Protoblock Player name is your fantasy identity, and will be displayed on the projections leaderboard. Claim your name! Choose your Twitter handle or fantasy team name";
        Column{
            anchors.fill: parent
            spacing: ProtoScreen.guToPx(2)
            Banner{
                id: aboutHeader
                width: parent.width
                height:   parent.height / 10
                backgroundColor:  Colors.primaryColor
                elevation: 2
                anchrosType: "verticalCenter"
                text: qsTr("Please pick a username")
                bold: true
                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
            }
            Image {
                id: logo
                source: "qrc:/logoFinal.png"
                width: parent.width / 1.07
                height: parent.height / 5.7
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label  {
                id: welcomeTxt
                text: defaultTxt
                width: parent.width / 1.07
                height: ProtoScreen.font(ProtoScreen.NORMAL)
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pixelSize: ProtoScreen.formFactor !== "desktop" ? ProtoScreen.font(ProtoScreen.SMALL) : ProtoScreen.font(ProtoScreen.NORMAL)
            }

            // Spacer
            Rectangle{width: 1;color: "transparent";height: ProtoScreen.guToPx(2)}
            LoginBox{
               width: parent.width / 1.07
               height: parent.height - ( logo.height + welcomeTxt.height + aboutHeader.height ) * 2
               anchors.horizontalCenter: parent.horizontalCenter
             }

            Label{
                id: errorLabel
                color: Theme.accentColor
                width: parent.width
                horizontalAlignment:  Text.AlignHCenter
                font.family: "Roboto"
                font.pixelSize: ProtoScreen.guToPx(2.75)
                wrapMode: Text.WordWrap
                onTextChanged: scale = scale +1
                Behavior on scale {
                    NumberAnimation{
                        duration: 1200 ; from: 0 ; to: 1 ; easing.type: Easing.OutBounce
                    }
                }
            }
        }//col


    function nameCheckBlank(s) {
        if ( s.length === 0 ) {
            return
        }
        else if ( s.length > 45) {
            errorString = "Sorry but that fantasy name is just to long. Please try again."
            accountErrorDialog.toggle()
        }
        else {
            MiddleMan.checkname(s)
        }
    }

    SequentialAnimation{
        id: welcomeTxtAnimation
        NumberAnimation { target: welcomeTxt ; property: "opacity"; from: 1; to: 0; duration: 1; }
        NumberAnimation { target: welcomeTxt ; property: "opacity"; from: 0; to: 1; duration: 600; }
    }
}
