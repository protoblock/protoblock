import QtQuick 2.0
import ProRotoQml.Protoblock 1.0


import Material 1.0
import Material.ListItems 1.0 as ListItems
Dialog {
    title: "Account Settings"
    positiveButtonText: "back"
    visible: false
    onAccepted: toggle()
    onRejected: toggle()
    Text{
        width: parent.width
        height: Unit.dp(160)
        wrapMode: Text.WordWrap
        text:  errorString
    }

    Column{
        anchors.fill: parent
        spacing: 2
        ListItems.Standard {
            text: "Account Settings "
            elevation: 1
        }

        ListItems.Standard {
//            text: userSetttings.secert3File
            elevation: 1
        }

        ListItems.Standard {
            text: "Public Key: " /*+ userSetttings.publicKey*/
            elevation: 1
        }

        ListItems.Standard {
            text: "Private Key: "/* + userSetttings.privateKey*/
            elevation: 1
        }

        ListItems.Standard{
//            text: MiddleMan.nameStatuses(0)
            elevation: 1
        }
    }
}

