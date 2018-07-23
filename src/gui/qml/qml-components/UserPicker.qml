import QtQuick 2.3

import Material 1.0

import ProRotoQml.Theme 1.0

Item {
    property alias model: menuField.model
    MaterialComboBox{
        id:menuField
        //            model: MiddleMan.goodList()
        //            textRole: "uname"
        //            Material.Label{
        //            text: realRoot.uname
        //            verticalAlignment: navDrawer.enabled ? Text.AlignVCenter : Text.AlignTop
        //            font{
        //                family: "Roboto"
        //                bold: true
        //                pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
        //            }
        //            color: "white"
        //        Component.onCompleted: console.log("Here is the list +  " MiddleMan.goodList() )

    }
}
