import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1

Item {
    property bool ss: false

    id: listsearch
    width: parent.width
    height: parent.height

    ListView {
        width: parent.width
        height: parent.height - ProtoScreen.guToPx(5)
        implicitWidth: parent.width
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        model: MiddleMan.pPlayerSymbolsModel
        //model: MiddleMan.pPlayerQuoteSliceViewFilterProxyModel

        headerPositioning: ListView.OverlayHeader

        delegate: RowLayout {
            width: parent.width
            Layout.margins: 0
            height: ProtoScreen.guToPx(4)
                Material.IconButton{
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Layout.preferredWidth: (parent.width / 8.0)
                    color: "black"
                    onClicked: {
                        MiddleMan.addTradingSymbol(model.symbol)
                    }
                    action: Material.Action {
                        iconName: "awesome/plus"
                    }
                    size: ProtoScreen.guToPx(2)

                }

                Label {
                    text: model.symbol
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: parent.width / 4.0
                    font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

                }
                Label {
                    text: !model.fullname ? MiddleMan.fillPlayerBase(model.symbol, model.playerid) : model.fullname
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
        //                                Layout.preferredWidth: (parent.width / 4) - 2
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: parent.width * .75
                    font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

                }
        }


        header: Material.Card {
            z: 2
            width: parent.width
            Layout.margins: 0
            height: ProtoScreen.guToPx(5)

            RowLayout {
            width: parent.width
            Layout.margins: 0
            height: ProtoScreen.guToPx(5)
            Material.IconButton{
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * .20)
                color: "black"
                onClicked: {
                    listsearch.ss = false
                }
                action: Material.Action {
                    iconName: "awesome/minus"
                }
                size: ProtoScreen.guToPx(2.5)
            }

            Material.Card {
                height: parent.height
                implicitWidth:parent.width
                TextField {
                    height: parent.height
                    anchors.leftMargin: ProtoScreen.guToPx(.5)
                    width: parent.width * 60
                    //height: parent.height
                    font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                    font.capitalization: Font.AllUppercase
                    placeholderText: "search..."
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhUppercaseOnly;
                    onTextChanged: {
//                        if ( length > 1 )
//                           MiddleMan.pPlayerSymbolsModel.filterString = text
                    }

                    onEditingFinished: {
                        MiddleMan.pPlayerSymbolsModel.filterString = text.toUpperCase()

                    }


                    //helperText: "player name search"
                    enabled: true//false//styleData.column === 1
                    visible: true//false//styleData.column === 1
                    activeFocusOnPress: true

                }
            }
            }
        }
    }
}
