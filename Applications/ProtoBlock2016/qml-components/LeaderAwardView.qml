import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1

import ProRotoQml.Theme 1.0


Item {
    id: lawardv
//    property alias modelPick:  tvl.model
    anchors.fill: parent
    Item {
        width: parent.width
        height: parent.height

        TableView {
            id: tvl
            Component.onCompleted: {
                if ( model ) {
//                    resizeColumnsToContents()
                    model.sortAgain("award",Qt.DescendingOrder)
                }
            }

            sortIndicatorColumn: 1
            highlightOnFocus:   false
            height: parent.height //- ProtoScreen.guToPx(5)
            implicitWidth: parent.width

            selectionMode: SelectionMode.NoSelection
            model: MiddleMan.pResultSelectedModel //? 0 : MiddleMan.pResultSelectedModel
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                model.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
                model.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }


            rowDelegate: Rectangle {
               height: ProtoScreen.guToPx(3)
               color: styleData.alternate?"#f5f5f5":"transparent"
            }

            headerDelegate:  Rectangle {
                width: parent.width
                height: ProtoScreen.guToPx(8)
                implicitWidth: textItem2.implicitWidth
                color: "white"
                Material.Card {
                    width: parent.width
                    height: parent.height * .50
                    backgroundColor: themeroot.theme.primaryColor
                    anchors.bottom: parent.bottom
                    radius: 1
                    border.color:
                        styleData.column  === tvl.sortIndicatorColumn ? themeroot.theme.accentColor : "black"

                    Material.Label {
                        id: textItem2
                        text: " " + styleData.value + " "
                        anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        elide: Text.ElideRight
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                        font.family: styleData.column ===  2 ? fontfamFB : "Roboto"

                    }
                }
            }

            frameVisible: false

            TableViewColumn {
                role: "name"
                title: "Fantasy Name"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(15)
                delegate: Material.Card {
                        anchors.margins: 0
                        id: mcard
                        anchors.fill: parent
                        flat: true
                        backgroundColor: MiddleMan.isMyName(styleData.value) ?
                                             (styleData.value !== uname ? Material.Theme.light.textColor : themeroot.theme.accentColor )
                                           : "#AFE1FF"

                        Material.Label {
                            id: tx
                            anchors.fill: parent
                            anchors.centerIn: parent
                            text: styleData.value
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillHeight: true
                            Layout.fillWidth: false
                            color: MiddleMan.isMyName(styleData.value) ?
                                       (styleData.value !== uname ? themeroot.theme.accentColor : Material.Theme.light.textColor)
                                     : Material.Theme.light.textColor
                        }

                }

            }

            TableViewColumn{
                role: "proj"
                title: "Projection"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(10)
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: styleData.value
                }
            }

            TableViewColumn{
                role: "award"
                title: "Award ƑɃ"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(10)


                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)
                    font.family: fontfamFB
                }
            }
        }
    }

    Connections {
        target: MiddleMan.pResultSelectedModel

        onSourceModelChanged: {
            console.log(" finally model changd");
            tvl.model = MiddleMan.pResultSelectedModel
        }

//        onPResultSelectedModelChanged: {
//            console.log(" finally model changd");
//            tvl.model = MiddleMan.pResultSelectedModel
//        }


    }
}
