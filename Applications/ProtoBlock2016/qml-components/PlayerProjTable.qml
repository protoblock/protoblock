import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import ProRotoQml.Protoblock 1.0


Item {
    id: topw
    anchors.fill: parent
//    TableView{
//        id: statsView
//        frameVisible: false
//        sortIndicatorVisible: true
//        sortIndicatorOrder: Qt.AscendingOrder
////        onSortIndicatorColumnChanged: {
////            proxyModel.sortAgain(getColumn(sortIndicatorColumn).role, Qt.DescendingOrder)
////        }
//        width: parent.width / 1.07
//        height: parent.height
////        anchors.top: searchBox.bottom;
////        anchors.topMargin: 5
//        anchors.horizontalCenter: parent.horizontalCenter
//        model:

////            SortModel {
////            id: proxyModel
////            source: playerStatsModel.count() > 0 ? playerStatsModel : null
////            sortOrder: statsView.sortIndicatorOrder
////            sortCaseSensitivity: Qt.CaseInsensitive
////            sortRole: statsView.count > 0 ? statsView.getColumn(statsView.sortIndicatorColumn).role : ""

////            filterString: "*" + searchBox.text + "*"
////            filterSyntax: SortModel.WildMaterial.Card
////            filterCaseSensitivity: Qt.CaseInsensitive
////        }

    signal indrop
    signal donedrop


    ComboBox {
        model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
//            height: tv.height
        enabled: true
        currentIndex: 0
        onCurrentTextChanged: {
           MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
        }
        id: cb
    }


//    property var widths: [0,0,0,0,0,0,0]



//    MouseArea{
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onClicked: {
//            console.log("ListView is clicked")
//            mouse.accepted = false
//        }
//    }
//    TableView {
//        id: tv0

//        anchors.top: cb.bottom
//        width: tv.width
//        height: ProtoScreen.guToPx(3)

//        model: MiddleMan.pProjectionsViewFilterProxyModel.columnCount


//        itemDelegate: Material.Card{
//            id: dropDown
//            width: widths[styleData.column]
//            height: ProtoScreen.guToPx(3)
//            backgroundColor: themeroot.theme.primaryColor
//            radius: 0

//            ComboBox {
//                id: cbc
//                model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
//        //            height: tv.height
////                    enabled: false
//                currentIndex: 0
////                    visible: false
//                anchors.fill: parent
//                onCurrentTextChanged: {
//                   MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
//                }
//            }
////            Label{
////                id: textItem
////                text: styleData.value
////                anchors.fill: parent
////                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
////                elide: Text.ElideRight
////                wrapMode: Text.WordWrap
////                verticalAlignment: Text.AlignVCenter
////                horizontalAlignment: Text.AlignHCenter
////                color: "white"
////            }

//            onWidthChanged: { console.log("y no FUCKER ");
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    console.log(" ? YES FUCKER ")
//                }
//            }

//            function tmyMethod() {
//                console.log(" col " + styleData.column + " " + widths[styleData.column] )

//                dropDown.width = Qt.binding(function() {
//                    console.log(" col " + styleData.column + " " + widths[styleData.column] )

//                    return widths[styleData.column];
//                })
//            }


//            Component.onCompleted: {
//                topw.wchanged.connect(tmyMethod)
//            }

//        }


//        frameVisible: true
//        TableViewColumn{
//            role: "fullname"
//            title: "Name"
//            horizontalAlignment : Text.AlignHCenter
//            width: widths[0]

//        }
//        TableViewColumn{
//            role: "teamid"
//            title: "Team"
//            horizontalAlignment : Text.AlignHCenter
//            width: widths[1]

//        }
//        TableViewColumn{
//            role: "pos"
//            title: "Pos"
//            horizontalAlignment : Text.AlignHCenter

//        }
//        TableViewColumn{
//            role: "status"
//            title: "status"
//            horizontalAlignment : Text.AlignHCenter

//        }

//        TableViewColumn{
//            role: "projection"
//            title: "projection"

//            horizontalAlignment : Text.AlignHCenter
//        }
//    }

    Item {
        anchors.top: cb.bottom
        width: parent.width
        height: parent.height - cb.height

        TableView {
            id: tv
            Component.onCompleted: {
    //            resizeColumnsToContents()
            }

            highlightOnFocus:   false
            anchors.fill: parent

//            anchors.top: parent.top
//            width: parent.width
//            height: parent.heigh//t - tv0.height
            model: MiddleMan.pProjectionsViewFilterProxyModel
            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.AscendingOrder
            onSortIndicatorColumnChanged: {
    //            accountErrorDialog.show();
    //            MiddleMan.pProjectionsViewFilterProxyModel.sort(sortIndicatorColumn, sortIndicatorOrder)
                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
    //            MiddleMan.pProjectionsViewFilterProxyModel.sort(sortIndicatorColumn, sortIndicatorOrder)

                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            headerDelegate:
    //            ComboBox {
    //                model: 40
    ////                anchors.fill: parent
    //                editable: true
    //                            width: parent.width
    //                            height: ProtoScreen.guToPx(3)

    //            }
            Item {
            width: parent.width
            height: ProtoScreen.guToPx(6)

            Rectangle {
                id: rec
                height: parent.height * .60
                width: parent.width
                color: styleData.column < 3 ? "white" : "grey"
                anchors.top: parent.top

                ComboBox {
                    id: cbc
                    model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
            //            height: tv.height
                        enabled: styleData.column === 2
                    currentIndex: 0
                        visible: styleData.column === 2
                    anchors.fill: parent
                    onCurrentTextChanged: {
                       MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
                    }
                }

                MouseArea {
                    enabled: styleData.column !== 2
    //                z: -1
                    anchors.fill: parent
                    onClicked: {
                        console.log(" YES FUCKER ")
                    }
                }


            }

            Card {
                width: parent.width
                height: parent.height * .40
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottom: parent.bottom
                radius: 0
                border.color:
                    styleData.column  === tv.sortIndicatorColumn ? "green" : "black"


                Label{
                    id: textItem2
                    text: styleData.value
                    anchors.fill: parent
    //                width: parent.width
    //                height: parent.height * .40
    //                anchors.bottom: parent.bottom
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }
//            Component.onCompleted: {
//                widths[styleData.column] = width
//            }

//            onWidthChanged: { console.log(" no FUCKER ");
//                widths[styleData.column] = width
//                topw.wchanged()
//            }

            function tmyMethod() {
//                console.log(" col " + styleData.column + " " + widths[styleData.column] )

                rec.color = Qt.binding(function() {
//                    console.log(" col " + styleData.column + " " + widths[styleData.column] )
                    if ( styleData.column >= 3)
                        return "green"
                    else
                        return rec.color
                })
            }

            function donedropM() {
                rec.color = Qt.binding(function() {
                    if ( styleData.column >= 3)
                        return "grey"
                    else
                        return rec.color
                })
            }


            Component.onCompleted: {
                topw.indrop.connect(tmyMethod)
                topw.donedrop.connect(donedropM)
            }

        }

            frameVisible: true
            TableViewColumn{
                role: "fullname"
                title: "Name"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "teamid"
                title: "Team"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "pos"
                title: "Pos"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "status"
                title: "status"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "projection"
                title: "projection"

                horizontalAlignment : Text.AlignHCenter
                delegate:
                    ComboBox {
                        model: 40
                        anchors.fill: parent
                        editable: true
                    }
            }
        }
    }
}
