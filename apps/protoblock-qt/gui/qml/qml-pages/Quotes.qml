import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

//import ProRotoQml.Protoblock 1.0
import ProtoblockQml.Theme 1.0
//import ProRotoQml.Models 1.0

Item {
    Component.onCompleted: {
         pageHelper.title = "Quote Page"

        if ( !realRoot.reloadrowquote )
            realRoot.reloadrowquote = true
        else {
            MiddleMan.rowMarketGet()
            realRoot.reloadrowquote = false
        }

//        if ( realRoot.reloadorderpos ) {
//            MiddleMan.getOrderPos()
//            realRoot.reloadorderpos = false
//        }
    }
    Component.onDestruction: {
        realRoot.reloadrowquote = true
    }

    Scrollbar{flickableItem: fl}
    Flickable{
        id: fl
        width: parent.width / 1.20
        height:    parent.height
        contentHeight: parent.height * 3
        contentWidth: parent.width / 1.20
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        anchors.horizontalCenter: parent.horizontalCenter
    Card{
        id: cccc
        width: parent.width / 1.07
        height: parent.height
        elevation: 0
//            anchors.centerIn: parent
        anchors{
            top: parent.top
            topMargin:ProtoScreen.guToPx(.5)
            horizontalCenter: parent.horizontalCenter
        }

        // spacer
        Rectangle{width: 1; height: ProtoScreen.guToPx(1);color: "transparent"}

        Banner {
            id: mybanner
//                height: parent.height
//                anchrosType: "center"
            anchors.horizontalCenter: parent.horizontalCenter
            fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
            bold: true
            text: "Prices - 2016 Rest of the Way (0/16)"
            color: "white"
            backgroundColor: themeroot.theme.primaryColor
            helpShown: true
            helperHeader: "2016 Rest of the Way"
            helperTxt: "List of active markets for Season long trading. This is the 3rd level of the Protoblock Fantasy Football skill test. Click a player to see in-depth market and to trade."
            width: fl.width / 1.07
            height: ProtoScreen.guToPx(6)
            anchors.bottomMargin:ProtoScreen.guToPx(.5)
            anchors.topMargin:ProtoScreen.guToPx(.5)
            anchors.top: parent.top
        }

        ListView{
            anchors.margins: 1
            width: cccc.width
            height: cccc.height - ProtoScreen.guToPx(6)
            id: playersListView
            anchors.top: mybanner.bottom
            model: MiddleMan.pPlayerQuoteSliceModel
            footer: Column {
                property string mysymbol: ""
                width: parent.width
                anchors.topMargin: ProtoScreen.guToPx(1)
                spacing: ProtoScreen.guToPx(4)
                TextField {
                    id: symbol
                    width: parent.width / 1.07
                    font.pixelSize: ProtoScreen.font(ProtoScreen.MEDIUM)
                    font.family: "Default"
                    helperText: "Enter in new symbol"
                    anchors.horizontalCenter: parent.horizontalCenter
                    inputMethodHints: Qt.ImhNoPredictiveText;
                }

                        Button{
                            id: quotebutton
                            text: "Get Quote"
                            width: parent.width / 1.07
                            elevation: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            onClicked: {
                                if ( mysymbol !== symbol.text) {
                                    mysymbol = symbol.text;
                                    MiddleMan.startDepth(mysymbol)
                                }

//                                rootLoader.source = "qrc:/Orders.qml"
                            }
                            backgroundColor: themeroot.theme.primaryColor
                        }
                    }

            delegate:
                ListItems.Subtitled{

                elevation: 1 // ListView.isCurrentItem ? 2 : 1
                backgroundColor: "white"//  ListView.isCurrentItem ? themeroot.theme.accentColor : "white"
                width: parent.width
                text:   model.fullname + " (" + model.position +")"
                itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
//                secondaryItem.anchors.left: itemSubLabel.anchors.right
                subText:{
                    if ( model.volume > 0 )
                    "Symbol: " + model.symbol
                            + " | Volume: "+model.volume
                    else  "Symbol: " + model.symbol
                }
                secondaryItem: RowLayout{
                    width: ProtoScreen.guToPx(32)
                    height: ProtoScreen.guToPx(8)
                    Label{
                        id: las
                        text: model.lastprice === 0 ?  "" : ("Price: " + model.lastprice.toString() + arrow.text)

                        color: model.updown < 0 ? Colors.red :
                                 model.updown > 0 ? Colors.green : "black"


                        Layout.fillHeight: true
                        Layout.fillWidth:  true
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        id: arrow
                        text: (model.updown < 0) ? " ↓" : (model.updown > 0) ? " ↑" : " ";
                        color: "transparent"
                    }

                    Label{
                        text: model.change === 0 ? "" : model.change.toString()
                        Layout.fillHeight: true
                        Layout.fillWidth:  false
                        verticalAlignment: Text.AlignVCenter
                    }

                    Icon{
                        enabled: model.change !== 0
                        Layout.fillWidth:  false
                        Layout.fillHeight: true
                        hasColor:true
                        color: { model.change < 0 ? Colors.red :
                                 model.change > 0 ? Colors.green : "transparent"
                        }
                        source: {
                            if (model.change < 0 ){
                                "qrc:/icons/ic_trending_down.png"
                            }
                            else if (model.change > 0 )
                                    "qrc:/icons/ic_trending_up.png"
                            else ""

                        }
                    }
                }
                action: Icon{
                    hasColor:false
                    source: "qrc:/"+ model.teamid+".PNG"
                    width: ProtoScreen.guToPx(6)
                    height: width
                    smooth: true
                }
                onClicked: {
//                            rootLoader.binder = mybinder

//                            MiddleMan.set_pPlayerQuoteSliceModelItem(playersListView.model[playersListView.currentIndex]);
                    MiddleMan.startDepth(model.playerid)
//                                                        realRoot.holdvar = model
//                            depthload.source = "qrc:/DepthTrader.qml"
//                            depthload1.source = "qrc:/DepthTrader.qml"
//                            depthload1.source = "qrc:/Projections.qml"

                     rootLoader.source = "qrc:/TradingFloor.qml"
//                            var dataViewer = Qt.createComponent("qrc:/DepthTrader.qml").
//                                createObject(realRoot, {inplay: playersListView.model[playersListView.currentIndex]});
//                              dataViewer.show()

                }


            }
            }

            Scrollbar{flickableItem: playersListView }

        }

    }
}



    /*
                        //                        onClicked: {
//                            realRoot.playerInView = model.playerid

//                            buySell.bannerText = model.firstname  + " " +model.lastname

//                            //   fixme these should be arrays

//                            sellCard.setprice = model.ask
//                            console.log(" ask " + model.ask)

//                            buycard.setprice = model.bid
//                            console.log(" bid " + model.bid)


//                            console.log(" buy " + buycard.price)
////                                    console.log(" buy " +  buycardpriceCombo.currentIndex)
//                            console.log(" sell " + sellCard.price)
////                                    console.log(" sell " + sellCard.priceCombo.currentIndex)

//                            playersListView.currentIndex = index
//                            MiddleMan.startDepth(model.playerid)

//                        }
*/

//    Connections {
//        target: MiddleMan
//        onLeaderBoardchanged: {
////            leaderboard.model = MiddleMan.allNamesList()
//        }
//    }


/*
    Dialog{
        id: filterOption
        contentMargins: 0
        ExclusiveGroup { id: optionGroup }
        ExclusiveGroup { id: viewGroup }
        Column{
            anchors.fill: parent
            spacing: ProtoScreen.guToPx(1)
            Banner{
                text: qsTr("Filters")
                width:parent.width
                backgroundColor: themeroot.theme.primaryColor
            }

            Label{
                text: qsTr("View Type")
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
                font.bold:  true
            }
            Rectangle{
                color: "grey"
                height:    1
                width: parent.width / 1.07;
                anchors.horizontalCenter: parent.horizontalCenter
            }
            RowLayout{
                width: parent.width / 1.07
                height:    checkOne.height
                anchors.horizontalCenter: parent.horizontalCenter
//                RadioButton {
//                    Layout.fillWidth: true
//                    checked: true
//                    text: qsTr("CoverFlow")
//                    onClicked:  viewInFocus = two
//                    exclusiveGroup: viewGroup
//                }
                RadioButton {
                    Layout.fillWidth: true
                    text: qsTr("Classic")
                    onClicked:  {viewInFocus = one; filterOption.close()}
                    exclusiveGroup: viewGroup
                }
                RadioButton {
                    Layout.fillWidth: true
                    text: qsTr("Grid")
                    onClicked:{ viewInFocus = three; filterOption.close()}
                    exclusiveGroup: viewGroup
                }
                RadioButton {
                    Layout.fillWidth: true
                    text: qsTr("List")
                    onClicked: {viewInFocus = four;; filterOption.close()}
                    exclusiveGroup: viewGroup
                }
            }



            Label{
                text: qsTr("Open Or Locked")
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
                font.bold:  true
            }
          Rectangle{height: 1; color:"#88c3c3c3";width: parent.width / 1.07;anchors.horizontalCenter: parent.horizontalCenter}

            RowLayout{
                width: parent.width / 1.07
                height:    checkOne.height
                anchors.horizontalCenter: parent.horizontalCenter

                RadioButton {
                    id: checkOne
                    Layout.fillWidth: true
                    checked: true
                    text: "open"
                    canToggle: true
                    exclusiveGroup: optionGroup
                }
                RadioButton {
                    Layout.fillWidth: true
                    text: "locked"
                    exclusiveGroup: optionGroup
                }
            }

            Label{
                text: qsTr("Teams and Postions")
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
                font.bold:  true
            }
            Rectangle{height: 1; color:"#88c3c3c3";width: parent.width / 1.07;anchors.horizontalCenter: parent.horizontalCenter}

            RowLayout{
                width: parent.width / 1.07
                height:    checkOne.height
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: ProtoScreen.guToPx(1)
                MenuField {
                    Layout.fillWidth: true
                    width: parent.width / 3
                    // fixme model is team mode or somthing in material root
                    model: ["ARI", "BUF", "NE"]
                }
                MenuField {
                    Layout.fillWidth: true
                    width: parent.width / 5
                    // fixme model is team mode or somthing in material root
                    model: ["ALL", "QB", "RB"]
                }
            }


            Label{
                text: qsTr("Market")
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
                font.bold:  true
            }
            Rectangle{
                height:    1
                width: parent.width / 1.07;
                color: "#88c3c3c3"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            RowLayout{
                width: parent.width / 1.07
                height:    checkOne.height
                anchors.horizontalCenter: parent.horizontalCenter
                RadioButton {
                    Layout.fillWidth: true
                    text: "Active"
                    canToggle: true
                    exclusiveGroup: optionGroup
                }

                RadioButton {
                    Layout.fillWidth: true
                    enabled: false
                    text: "Liquid"
                }
                RadioButton {
                    Layout.fillWidth: true
                    enabled: false
                    text: "Movers"
                }
            }
        }
    }
*/
