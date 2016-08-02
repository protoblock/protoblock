import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Models 1.0

FocusScope {
    property Component viewInFocus: four
    Scrollbar{flickableItem: tF}
    Component.onCompleted: {
        MiddleMan.rowMarketGet()
    }
    Flickable{
        id: tF
        width: parent.width
        height:    parent.height
        contentHeight: parent.height * 2
        contentWidth: parent.width
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        Card {
            width: parent.width
            height: parent.height
            elevation: 0
            anchors.centerIn: parent

            Column{
                width: parent.width
                height: parent.height
                anchors.fill: parent
                spacing: ProtoScreen.guToPx(1.25)

                // spacer
                Rectangle{width: 1; height: ProtoScreen.guToPx(1);color: "transparent"}
                Row {
                    width: tF.width / 1.07
                    height: parent.height / 6
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: ProtoScreen.guToPx(1)


                    //The buy and sell cards in the top left
                    CardWithContent{
                        id: buySell
                        width: parent.width /1.7
                        height: parent.height
                        anchrosType: "center"
                        bannerText: "select player to trade"
                        helperHeader: "Buying and Selling Help"
                        helperTxt: ""
                        helpShown: true
                        object:
                            Row{
                            width: parent.width
                            height:parent.height
                            BuyTrading {
                                id: buycard
                                width: parent.width / 2
                                height: parent.height
                            }
                            BuyTrading {
                                id: sellCard
                                width: parent.width / 2
                                height: parent.height
                                isbuyCard: false

                            }
                        }
                    }

                    CardWithContent{
                        width: parent.width  - buySell.width
                        height: parent.height
                        bannerText: "Market Depth"
                        anchrosType: "center"
                        helperHeader: "Market Depth Help"
                        helperTxt: " "
                        helpShown: true
                        object:
                            ListView{
                            id: leaderboard
                            width: parent.width
                            height: parent.height
                            clip: true
                            model:  MiddleMan.pDepthMarketModel
                            header: RowLayout{
                                width: parent.width
                                height: ProtoScreen.guToPx(2)
                                spacing: 0
                                Rectangle{width: 2; height: 1;color: "transparent"}
                                Repeater{
                                    model: ["Bid Size","Bid","Ask","Ask Size"]
                                    Card{
                                        Layout.fillHeight: true
                                        Layout.fillWidth: false
                                        Layout.preferredWidth: (parent.width / 4) - 2
                                        border.color:"black"
                                        backgroundColor: Colors.blue
                                        Label{
                                            anchors.centerIn: parent
                                            text: modelData
                                            font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                                            color: "white"
                                        }
                                    }
                                }
                            }
                            remove: Transition {
                                ParallelAnimation{
                                    NumberAnimation { property: "opacity";from:.1; to: 9; duration: 400 }
                                }
                            }
                            add: Transition {
                                ParallelAnimation{
                                    NumberAnimation { property: "opacity";from:.9; to: 1; duration: 400 }
                                    //                                    NumberAnimation{property: "scale";from:.98; to:1; duration:200; easing.type: Easing.OutBack}
                                }
                            }
                            delegate:
                                MarketDepthCard{
                                height: ProtoScreen.guToPx(4)
                                elevation: 2
                                width: parent.width
                                buySize:  model.bidsize
                                buy: model.bid
                                sellSize: model.asksize
                                sell: model.ask
                            }
                        }// listView
                    }
                }


                Loader{
                    id: viewOne
                    anchors.horizontalCenter: parent.horizontalCenter
                    height:tF.height / 2.2
                    width: parent.width / 1.07
                    sourceComponent: viewInFocus

                }

                // Componets for the bottom middle cards and options

//                Component{
//                    id:one
//                    CardWithContent{
//                        bannerObject:
//                            RowLayout{
//                            width: parent.width / 1.07
//                            height: parent.height
//                            anchors.centerIn: parent
//                            Label{
//                                text: qsTr("Available Players")
//                                color: "white"
//                                font.pixelSize: ProtoScreen.font( ProtoScreen.LARGE)
//                                font.bold: true
//                                Layout.fillWidth: true

//                            }

//                            IconButton{
//                                width: ProtoScreen.guToPx(6)
//                                hasColor: true
//                                color: "white"
//                                hoverAnimation: true
//                                iconSource: "qrc:/icons/action_settings.png"
//                                onClicked: filterOption.show()
//                            }
//                            TextField{
//                                id: filterTardis
//                                placeholderText: "Search . . . "
//                            }
//                        }
//                        object:
//                            TableView{
//                            id:  tableView
//                            anchors.fill: parent
//                            backgroundVisible: false
//                            alternatingRowColors: false

//                            onClicked: {
//                                MiddleMan.startDepth(model.get(row).playerid)
//                                realRoot.playerInView = model.get(row).playerid
//                                buySell.bannerText = model.get(row).firstname  + " " +model.get(row).lastname
//                                //   fixme these should be arrays
//                                buycard.buyPrice = model.get(row).bid
//                                buycard.sellPrice = model.get(row).ask

//                                sellCard.buyPrice = model.get(row).bid
//                                sellCard.sellPrice = model.get(row).ask
//                            }
//                            onActivated:{
//                                MiddleMan.startDepth( model.get(row).playerid)
//                                realRoot.playerInView = model.get(row).playerid
//                                buySell.bannerText = model.get(row).firstname  + " " +model.get(row).lastname
//                                //   fixme these should be arrays
//                                buycard.buyPrice = model.get(row).bid
//                                buycard.sellPrice = model.get(row).ask

//                                sellCard.buyPrice = model.get(row).bid
//                                sellCard.sellPrice = model.get(row).ask
//                            }

//                            //Buggy
//                            onCurrentRowChanged: {
//                                MiddleMan.startDepth(model.get(currentRow).playerid)
//                                var k = model.get(currentRow).playerid
//                                realRoot.playerInView =  k //model.get(currentRow).symbol
//                                buySell.bannerText = model.get(currentRow).firstname  + " " +model.get(currentRow).lastname
//                                //   fixme these should be arrays
//                                buycard.buyPrice = model.get(currentRow).bid
//                                buycard.sellPrice = model.get(currentRow).ask

//                                sellCard.buyPrice = model.get(currentRow).bid
//                                sellCard.sellPrice = model.get(currentRow).ask
//                            }
//                            sortIndicatorVisible: true
//                            sortIndicatorOrder: Qt.AscendingOrder
//                            onSortIndicatorColumnChanged: {
//                                sortModel.sortAgain(getColumn(sortIndicatorColumn).role, Qt.DescendingOrder)
//                            }
//                            model:SortModel {
//                                id: sortModel
//                                source: MiddleMan.pPlayerQuoteSliceModel
//                                sortOrder: tableView.sortIndicatorOrder
//                                sortCaseSensitivity: Qt.CaseInsensitive
//                                sortRole:  tableView.getColumn(tableView.sortIndicatorColumn).role
//                                filterString: "*" + filterTardis.text + "*"
//                                filterSyntax: SortModel.Wildcard
//                                filterCaseSensitivity: Qt.CaseInsensitive
//                            }
//                            style: MaterialTable{}
//                           // not sure about pnl and all that that was here last year is not in the model

//                            TableViewColumn {
//                                role: "lastprice";
//                                title: "Last";
//                            }
//                            TableViewColumn {
//                                role: "position";
//                                title: "Position";
//                            }

//                            TableViewColumn {
//                                role: "fullname";
//                                title: "NFL Player";
//                                width: ProtoScreen.guToPx(25)
//                            }

//                            TableViewColumn {
//                                role: "team_id";
//                                title: "Team";
//                            }
//                            TableViewColumn {
//                                role: "bidsize";
//                                title: "Bid Size";

//                            }
//                            TableViewColumn {
//                                role: "bid";
//                                title: "Bid";

//                            }
//                            TableViewColumn {
//                                role: "ask";
//                                title: "Ask";

//                            }
//                            TableViewColumn {
//                                role: "asksize";
//                                title: "Ask Size";

//                            }
//                            TableViewColumn {
//                                role: "hi";
//                                title: "High";

//                            }
//                            TableViewColumn {
//                                role: "lo";
//                                title: "Low";

//                            }
//                            TableViewColumn {
//                                role: "volume";
//                                title: "Volume";

//                            }
//                            TableViewColumn {
//                                role: "change";
//                                title: "Change";

//                            }


//                        }
//                    }
//                }


//                Component{
//                    id:two
//                    CardWithContent{
//                        bannerObject:
//                            RowLayout{
//                            width: parent.width / 1.07
//                            height: parent.height
//                            anchors.centerIn: parent
//                            Label{
//                                text: qsTr("Available Players")
//                                color: "white"
//                                font.pixelSize: ProtoScreen.font( ProtoScreen.LARGE)
//                                font.bold: true
//                                Layout.fillWidth: true
//                            }

//                            IconButton{
//                                width: ProtoScreen.guToPx(6)
//                                iconSource: "qrc:/icons/action_settings.png"
//                                hasColor: true
//                                color: "white"
//                                onClicked: filterOption.show()
//                            }
//                            TextField{
//                                id: filterTard
//                                placeholderText: "Search . . . "
//                            }
//                        }
//                        object:
//                            Coverflow{
//                            clip:true
//                            backgroundColor: Colors.grey
//                            onItemInFocusChanged:{
//                                realRoot.playerInView = itemInFocus
//                            }
//                            anchors.fill: parent
//                            model: SortModel {
//                                source: MiddleMan.pPlayerQuoteSliceModel
//                                sortCaseSensitivity: Qt.CaseSensitive
//                                //maybe needs some work with linux style widecards ?
//                                filterString: "*"  +  filterTard.text  +  "*"
//                                filterSyntax: SortModel.Wildcard
//                                filterCaseSensitivity: Qt.CaseSensitive
//                            }
//                        }
//                    }
//                }

//                Component{
//                    id:three
//                    CardWithContent{
//                        bannerObject:
//                            RowLayout{
//                            width: parent.width / 1.07
//                            height: parent.height
//                            anchors.centerIn: parent
//                            Label{
//                                text: qsTr("Available Players")
//                                color: "white"
//                                font.pixelSize: ProtoScreen.font( ProtoScreen.LARGE)
//                                font.bold: true
//                                Layout.fillWidth: true

//                            }

//                            IconButton{
//                                width: ProtoScreen.guToPx(6)
//                                hasColor: true
//                                color: "white"
//                                iconSource: "qrc:/icons/action_settings.png"
//                                onClicked: filterOption.show()
//                            }
//                            TextField{
//                                id: filterTard
//                                placeholderText: "Search . . . "
//                            }
//                        }
//                        object:
//                            GridView {
//                            id: gView
//                            width: parent.width
//                            clip: true
//                            height: parent.height
//                            cellWidth:  width /3
//                            cellHeight: height / 1.07
//                            anchors.margins: ProtoScreen.guToPx(3)
//                            anchors.centerIn: parent
//                            remove: Transition {
//                                ParallelAnimation{
//                                    NumberAnimation { property: "opacity";from: 1 ; to: 0; duration: 1200 }
//                                    NumberAnimation{property: "scale";from: 1; to:0; duration:1200; easing.type: Easing.InBack}
//                                }
//                            }
//                            add: Transition {
//                                ParallelAnimation{
//                                    NumberAnimation { property: "opacity";from:0; to: 1; duration: 1200 }
//                                    NumberAnimation{property: "scale";from:0; to:1; duration:1200; easing.type: Easing.OutBack}
//                                }
//                            }
//                            model: SortModel {
//                                source: MiddleMan.pPlayerQuoteSliceModel
//                                sortCaseSensitivity: Qt.CaseSensitive
//                                //maybe needs some work with linux style widecards ?
//                                filterString: "*"  +  filterTard.text  +  "*"
//                                filterSyntax: SortModel.Wildcard
//                                filterCaseSensitivity: Qt.CaseSensitive
//                            }
//                            delegate: PlayerCard{
//                                cellWidth: gView.cellWidth / 1.07
//                                cellHeight:gView.cellHeight / 1.07
//                                width: cellWidth
//                                height: cellHeight
//                                first: model.firstname
//                                last: model.lastname
//                                playerid: model.playerid
//                                photoUrl: {
//                                    if (model.updown < 0 ){
//                                        "qrc:/icons/ic_trending_down.png"
//                                    }
//                                    else
//                                    {
//                                        if (model.change === 0 )
//                                        {
//                                            "qrc:/icons/ic_trending_flat.png"
//                                        }
//                                        else
//                                        {
//                                            "qrc:/icons/ic_trending_up.png"
//                                        }
//                                    }


//                                }
//                                team: model.team_id
//                                position: model.position

//                                bid: model.bid
//                                bidSize: model.bidsize
//                                ask: model.ask
//                                askSize: model.asksize
//                                volume: model.volume
//                                change: model.change
//                                lastPrice: model.lastprice

//                                MouseArea{
//                                    anchors.fill: parent
//                                    onClicked: {
//                                        MiddleMan.startDepth(model.playerid)
//                                        realRoot.playerInView = model.symbol
//                                        buySell.bannerText = model.firstname  + " " +model.lastname
//                                        //   fixme these should be arrays
//                                        buycard.buyPrice = model.bid
//                                        buycard.sellPrice = model.ask

//                                        sellCard.buyPrice = model.bid
//                                        sellCard.sellPrice = model.ask
//                                    }

//                                }
//                            }
//                        }
//                    }
//                }

                Component{
                    id:four
                    CardWithContent{
                        bannerObject:
                            RowLayout{
                            width: parent.width / 1.07
                            height: parent.height
                            anchors.centerIn: parent
                            Label{
                                text: qsTr("Available Players")
                                color: "white"
                                font.pixelSize: ProtoScreen.font( ProtoScreen.LARGE)
                                font.bold: true
                                Layout.fillWidth: true

                            }

                            IconButton{
                                width: ProtoScreen.guToPx(6)
                                hasColor: true
                                color: "white"
                                iconSource: "qrc:/icons/action_settings.png"
                                onClicked: filterOption.show()
                            }
                            TextField{
                                id: filterTards
                                placeholderText: "Search . . . "
                            }
                        }
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
//                            Keys.onDownPressed: model.currentIndex=index +1
//NOt till 5.7
                            //                            keyNavigationEnabled: true
                            model: SortModel {
                                source: MiddleMan.pPlayerQuoteSliceModel
                                sortCaseSensitivity: Qt.CaseSensitive
                                //maybe needs some work with linux style widecards ?
                                filterString: "*"  +  filterTards.text  +  "*"
                                filterSyntax: SortModel.Wildcard
                                filterCaseSensitivity: Qt.CaseSensitive
                            }
                            clip: true
                            delegate:
                                ListItems.Subtitled{

                                elevation:  ListView.isCurrentItem ? 2 : 1
                                backgroundColor:  ListView.isCurrentItem ? themeroot.theme.accentColor : "white"

//                                onBackgroundColorChanged: {
//                                    if ( ListView.isCurrentItem){
//                                        MiddleMan.startDepth(model.playerid)
//                                        realRoot.playerInView = model.symbol
//                                        buySell.bannerText = model.firstname  + " " +model.lastname
//                                        //   fixme these should be arrays
//                                        buycard.price = model.bid
//                                        sellCard.price = model.ask
//                                   }
//                                }
                                width: parent.width
                                text:   model.fullname + " (" + model.position +")"
                                subText:{
                                    "Bid Size: " + model.bidsize
                                            + " | Bid: "+model.bid
                                            + " | Ask: "+ model.ask
                                            + " | Ask Size: " + model.asksize
                                }
                                secondaryItem:
                                    RowLayout{
                                    width: ProtoScreen.guToPx(32)
                                    height: ProtoScreen.guToPx(8)
                                    Label{
                                        text: model.lastprice === 0 ? "" : model.lastprice
                                        Layout.fillHeight: true
                                        Layout.fillWidth:  true
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    Icon{
                                        Layout.fillWidth:  false
                                        Layout.fillHeight: true
                                        hasColor:true
                                        color: {
                                            if (model.updown < 0 ){
                                                Colors.red
                                            }else{
                                                if (model.change === 0 ){
                                                    "transparent"
                                                }else{
                                                    Colors.green
                                                }
                                            }
                                        }
                                        source: {
                                            if (model.updown < 0 ){
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
                                    Label{
                                        text: model.change === 0 ? "even" : model.change
                                        Layout.fillHeight: true
                                        Layout.fillWidth:  false
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                action: Icon{
                                    hasColor:false
                                    source: "qrc:/"+ model.team_id+".PNG"
                                    width: ProtoScreen.guToPx(6)
                                    height: width
                                }

                                onClicked: {
                                    realRoot.playerInView = model.playerid

                                    buySell.bannerText = model.firstname  + " " +model.lastname

                                    //   fixme these should be arrays

                                    sellCard.setprice = model.ask
                                    console.log(" ask " + model.ask)

                                    buycard.setprice = model.bid
                                    console.log(" bid " + model.bid)


                                    console.log(" buy " + buycard.price)
//                                    console.log(" buy " +  buycardpriceCombo.currentIndex)
                                    console.log(" sell " + sellCard.price)
//                                    console.log(" sell " + sellCard.priceCombo.currentIndex)

                                    playersListView.currentIndex = index
                                    MiddleMan.startDepth(model.playerid)

                                }
                            }
                        }
                    }
                }
            }
        }
    }




    //Help dialog
//    Dialog{
//        id: filterOption
//        contentMargins: 0
//        ExclusiveGroup { id: optionGroup }
//        ExclusiveGroup { id: viewGroup }
//        Column{
//            anchors.fill: parent
//            spacing: ProtoScreen.guToPx(1)
//            Banner{
//                text: qsTr("Filters")
//                width:parent.width
//                backgroundColor: themeroot.theme.primaryColor
//            }

//            Label{
//                text: qsTr("View Type")
//                width: parent.width / 1.07
//                anchors.horizontalCenter: parent.horizontalCenter
//                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
//                font.bold:  true
//            }
//            Rectangle{
//                color: "grey"
//                height:    1
//                width: parent.width / 1.07;
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//            RowLayout{
//                width: parent.width / 1.07
//                height:    checkOne.height
//                anchors.horizontalCenter: parent.horizontalCenter
////                RadioButton {
////                    Layout.fillWidth: true
////                    checked: true
////                    text: qsTr("CoverFlow")
////                    onClicked:  viewInFocus = two
////                    exclusiveGroup: viewGroup
////                }
//                RadioButton {
//                    Layout.fillWidth: true
//                    text: qsTr("Classic")
//                    onClicked:  {viewInFocus = one; filterOption.close()}
//                    exclusiveGroup: viewGroup
//                }
//                RadioButton {
//                    Layout.fillWidth: true
//                    text: qsTr("Grid")
//                    onClicked:{ viewInFocus = three; filterOption.close()}
//                    exclusiveGroup: viewGroup
//                }
//                RadioButton {
//                    Layout.fillWidth: true
//                    text: qsTr("List")
//                    onClicked: {viewInFocus = four;; filterOption.close()}
//                    exclusiveGroup: viewGroup
//                }
//            }



//            Label{
//                text: qsTr("Open Or Locked")
//                width: parent.width / 1.07
//                anchors.horizontalCenter: parent.horizontalCenter
//                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
//                font.bold:  true
//            }
//          Rectangle{height: 1; color:"#88c3c3c3";width: parent.width / 1.07;anchors.horizontalCenter: parent.horizontalCenter}

//            RowLayout{
//                width: parent.width / 1.07
//                height:    checkOne.height
//                anchors.horizontalCenter: parent.horizontalCenter

//                RadioButton {
//                    id: checkOne
//                    Layout.fillWidth: true
//                    checked: true
//                    text: "open"
//                    canToggle: true
//                    exclusiveGroup: optionGroup
//                }
//                RadioButton {
//                    Layout.fillWidth: true
//                    text: "locked"
//                    exclusiveGroup: optionGroup
//                }
//            }

//            Label{
//                text: qsTr("Teams and Postions")
//                width: parent.width / 1.07
//                anchors.horizontalCenter: parent.horizontalCenter
//                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
//                font.bold:  true
//            }
//            Rectangle{height: 1; color:"#88c3c3c3";width: parent.width / 1.07;anchors.horizontalCenter: parent.horizontalCenter}

//            RowLayout{
//                width: parent.width / 1.07
//                height:    checkOne.height
//                anchors.horizontalCenter: parent.horizontalCenter
//                spacing: ProtoScreen.guToPx(1)
//                MenuField {
//                    Layout.fillWidth: true
//                    width: parent.width / 3
//                    // fixme model is team mode or somthing in material root
//                    model: ["ARI", "BUF", "NE"]
//                }
//                MenuField {
//                    Layout.fillWidth: true
//                    width: parent.width / 5
//                    // fixme model is team mode or somthing in material root
//                    model: ["ALL", "QB", "RB"]
//                }
//            }


//            Label{
//                text: qsTr("Market")
//                width: parent.width / 1.07
//                anchors.horizontalCenter: parent.horizontalCenter
//                font.pixelSize: ProtoScreen.font( ProtoScreen.NORMAL)
//                font.bold:  true
//            }
//            Rectangle{
//                height:    1
//                width: parent.width / 1.07;
//                color: "#88c3c3c3"
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//            RowLayout{
//                width: parent.width / 1.07
//                height:    checkOne.height
//                anchors.horizontalCenter: parent.horizontalCenter
//                RadioButton {
//                    Layout.fillWidth: true
//                    text: "Active"
//                    canToggle: true
//                    exclusiveGroup: optionGroup
//                }

//                RadioButton {
//                    Layout.fillWidth: true
//                    enabled: false
//                    text: "Liquid"
//                }
//                RadioButton {
//                    Layout.fillWidth: true
//                    enabled: false
//                    text: "Movers"
//                }
//            }
//        }
//    }
}
