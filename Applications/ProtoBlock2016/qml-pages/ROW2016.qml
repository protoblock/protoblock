import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Models 1.0

Item {
    Component.onCompleted: {
         pageHelper.title = "2016 Futures Live"

        if ( !realRoot.reloadrowquote )
            realRoot.reloadrowquote = true
        else {
            MiddleMan.rowMarketGet()
            realRoot.reloadrowquote = false
        }
    }
    Scrollbar{flickableItem: fl}
    Flickable{
        id: fl
        width: parent.width
        height:    parent.height
        contentHeight: parent.height * 2
        contentWidth: parent.width
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        Card{
            width: parent.width
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

            CardWithContent {
                id: cwc
                height: parent.height
                anchrosType: "center"
                anchors.horizontalCenter: parent.horizontalCenter

                bannerText: "Active Markets - Rest of the Way 2016"
                backgroundColor: Theme.alpha(Colors.white, 1.5)
                helpShown: true
                helperHeader: "2016 Rest of the Way"
                helperTxt: "List of active markets for Season long trading. This is the 3rd level of the Protoblock Fantasy Football skill test. Click a player to see in-depth market and to trade."
                width: fl.width / 1.07
//                               width: parent.width
                /*
//                bannerObject:
//                    RowLayout{
//                    width: parent.width / 1.07
//                    height: parent.height
//                    anchors.centerIn: parent
//                    Label{
//                        text: qsTr(" Players")
//                        color: "white"
//                        font.pixelSize: ProtoScreen.font( ProtoScreen.LARGE)
//                        font.bold: true
//                        Layout.fillWidth: true

//                    }

//                    IconButton{
//                        width: ProtoScreen.guToPx(6)
//                        hasColor: true
//                        color: "white"
//                        iconSource: "qrc:/icons/action_settings.png"
//                        onClicked: filterOption.show()
//                    }
//                    TextField{
//                        id: filterTards
//                        placeholderText: "Search . . . "
//                    }
//                }*/

                object:
                    ListView{
                    id: playersListView
                    anchors.fill: parent
                    focus:   true
                    Keys.onDownPressed: {
                        console.log("Down pressed ")
                        if (playersListView.currentIndex + 1 < playersListView.count - 1)
                            playersListView.currentIndex += 1;
                    }
                    Keys.onUpPressed: {
                        console.log("Up pressed ")
                        if (playersListView.currentIndex - 1 >= 0)
                            playersListView.currentIndex -= 1;
                    }
                              // Keys.onDownPressed: model.currentIndex=index +1
                              //NOt till 5.7
                              //     keyNavigationEnabled: true
//                    model: SortModel {
//                        source: MiddleMan.pPlayerQuoteSliceModel
//                        sortCaseSensitivity: Qt.CaseSensitive
//                        //maybe needs some work with linux style widecards ?
//                        filterString: "*"  +  filterTards.text  +  "*"
//                        filterSyntax: SortModel.Wildcard
//                        filterCaseSensitivity: Qt.CaseSensitive
//                    }
                    clip: true
                    model: MiddleMan.pPlayerQuoteSliceModel
                    delegate:
                        ListItems.Subtitled{

                        elevation:  ListView.isCurrentItem ? 2 : 1
                        backgroundColor:  ListView.isCurrentItem ? themeroot.theme.accentColor : "white"

/*//                                onBackgroundColorChanged: {
//                                    if ( ListView.isCurrentItem){
//                                        MiddleMan.startDepth(model.playerid)
//                                        realRoot.playerInView = model.symbol
//                                        buySell.bannerText = model.firstname  + " " +model.lastname
//                                        //   fixme these should be arrays
//                                        buycard.price = model.bid
//                                        sellCard.price = model.ask
//                                   }
//                                }
*/
                        width: parent.width
                        text:   model.fullname + " (" + model.position +")"
                        subText:{
                            "Bid Size: " + model.bidsize
                                    + " | Bid: "+model.bid
                                    + " | Ask: "+ model.ask
                                    + " | Ask Size: " + model.asksize
                        }
                        secondaryItem: RowLayout{
                            width: ProtoScreen.guToPx(32)
                            height: ProtoScreen.guToPx(8)
                            Label{
                                id: las
                                text: model.lastprice + arrow.text

                                color: model.updown < 0 ? Colors.red :
                                         model.updown > 0 ? Colors.green : "black"


                                Layout.fillHeight: true
                                Layout.fillWidth:  true
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: arrow
                                text: (model.updown < 0) ? " ↓" : " ↑";
                                color: "transparent"
                            }

                            Label{
                                text: model.change
                                Layout.fillHeight: true
                                Layout.fillWidth:  false
                                verticalAlignment: Text.AlignVCenter
                            }

                            Icon{
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
                                    else
                                    {
                                        if (model.change === 0 )
                                        {
                                            "qrc:/icons/ic_trending_flat.png"
                                        }
                                        else
                                        {
                                            "qrc:/icons/ic_trending_up.png"
                                        }
                                    }
                                }
                            }
                        }
                        action: Icon{
                            hasColor:false
                            source: "qrc:/"+ model.team_id+".PNG"
                            width: ProtoScreen.guToPx(6)
                            height: width
                        }
                        onClicked: {
//                            MiddleMan.set_pPlayerQuoteSliceModelItem(playersListView.model[playersListView.currentIndex]);
                            MiddleMan.startDepth(model.playerid)
//                            depthload.source = "qrc:/DepthTrader.qml"
                            depthload.source = "qrc:/Projections.qml"

//                            var dataViewer = Qt.createComponent("qrc:/DepthTrader.qml").
//                                createObject(realRoot, {inplay: playersListView.model[playersListView.currentIndex]});
//                              dataViewer.show()
                        }
                        Loader{
                            id: depthload
//                            Binding {
//                              target: depthload.item
//                              property: "inplay"
//                              value:  model
//                              when: depthload.status === Loader.Ready
//                            }
                       }

                    }
                }
            }
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
