import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProtoblockQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1

Item {
    property bool ss: false
    property bool wk: true

    id: listsearch
    width: parent.width
    height: parent.height

    ListView {
        width: parent.width
        height: parent.height
        implicitWidth: parent.width
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        model: MiddleMan.pPlayerSymbolsModel
        //model: MiddleMan.pPlayerQuoteSliceViewFilterProxyModel

        headerPositioning: ListView.OverlayHeader

        delegate: RowLayout {
            width: parent.width
            height: ProtoScreen.guToPx(4)
                Material.IconButton{
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Layout.preferredWidth: (parent.width / 8.0)
                    color: "black"
                    onClicked: {
                        console.log("adding trading symbols")
                        MiddleMan.addTradingSymbol(model.symbol,listsearch.wk)
                    }
                    action: Material.Action {
                        iconName: "awesome/plus"
                    }
                    size: ProtoScreen.guToPx(2)

                }

                Material.Label {
                    text: model.symbol
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: parent.width / 4.0
                    font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

                }
                Material.Label {
//                    text: !model.fullname ? MiddleMan.fillPlayerBase(model.symbol, model.playerid) : model.fullname
                    text: MiddleMan.checkFullName(model.fullname,model.symbol)
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
        //                                Layout.preferredWidth: (parent.width / 4) - 2
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: parent.width * .75
                    font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

                }
        }

        header: headerdel

    }

    Component {
        id: headerdel
//    Rectangle {
//    z: 2
//    width: parent.width
//    Layout.margins: 0
//    height: ProtoScreen.guToPx(5)
//    color: "transparent"
//    anchors.fill: parent
////            anchors.margins: ProtoScreen.guToPx(.25)

        Rectangle {
            z: 2
            width: parent.width
            height: ProtoScreen.guToPx(8)
//            anchors.margins: ProtoScreen.guToPx(.5)
            color: "white"
            Rectangle {
                color: "transparent"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: ProtoScreen.guToPx(.25)
                width: parent.width * .75
                height: parent.height
                RowLayout {
                    anchors.centerIn: parent
                    spacing: ProtoScreen.guToPx(1)
                    Material.Label {
                        text: "Symbol:"
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    }

                    Material.TextField {
                    id: sf
    //                anchors.left: parent.left
    //                anchors.top: parent.top
    //                height: parent.height
    //                Layout.fillWidth: true
    //                Layout.fillHeight: false
    ////                anchors.verticalCenter: parent.verticalCenter
    //                anchors.leftMargin: ProtoScreen.guToPx(.5)
    //    //                    anchors.centerIn: parent
    ////                Layout.preferredWidth: (parent.width * .80)
    //                width: ProtoScreen.guToPx(12)
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    font.capitalization: Font.AllUppercase
                    placeholderText: "Search..."
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhUppercaseOnly;
    //                onTextChanged: {
    //    //                        if ( length > 1 )
    //    //                           MiddleMan.pPlayerSymbolsModel.filterString = text
    //                }

                    onAccepted: {
                        MiddleMan.pPlayerSymbolsModel.filterString = text.toUpperCase()
    //                    focus= true
                    }

                    helperText: "Initials + Position"
                    enabled: true//false//styleData.column === 1
                    visible: true//false//styleData.column === 1
                    activeFocusOnPress: true
                    focus: true
                }

                    Material.IconButton {
                        onClicked: {
                            MiddleMan.pPlayerSymbolsModel.filterString = sf.text.toUpperCase()
                        }
                        action: Material.Action {
                            iconName: "awesome/search"
                        }
                        size: ProtoScreen.guToPx(2)
                        hoverAnimation: true
                    }
                }
            }
            Material.IconButton {
                anchors.right: parent.right
                anchors.top: parent.top
//                Layout.fillHeight: true
//                Layout.fillWidth: false
//                Layout.preferredWidth: (parent.width * .20)
//                color: "black"
                onClicked: {
                    listsearch.ss = false
                }
                action: Material.Action {
                    iconName: "awesome/times"
                }
                size: ProtoScreen.guToPx(2.5)
            }
        }
    }
}
