import QtQuick 2.0
import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {

    Column{
        anchors.fill: parent
        spacing: 2
        ListItems.Standard {
            text: "Account: "
            elevation: 1
        }

        ListItems.Standard {
            text: userSetttings.secert3File
            elevation: 1
        }

        ListItems.Standard {
            text: "Public Key: " + userSetttings.publicKey
            elevation: 1
        }

        ListItems.Standard {
            text: "Private Key: " + userSetttings.privateKey
            elevation: 1
        }

        ListItems.Standard{
            text: MiddleMan.nameStatuses(0)
            elevation: 1
        }
    }
}
