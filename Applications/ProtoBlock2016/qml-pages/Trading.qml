import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtQuick.Controls 2.2 as Controls

//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.3


Item {
    property string seasontext: "NFL Season " + MiddleMan.theSeason + " - Week " + MiddleMan.theWeek // MiddleMan.seasonString + " 2017 Season Trading "
    property string liveorresult: MiddleMan.liveSync
    property string wkorrow: "WEEK-" + MiddleMan.theWeek

    property int theqmlweek: MiddleMan.theWeek
    property int theqmlseason: MiddleMan.theSeason

    property double totalopenpnl: MiddleMan.pROWTradingPositionsModel.totalopenpnl
                                                                            +
                                                             MiddleMan.pTradingPositionsModel.totalopenpnl

    Component.onCompleted: {
        pageHelper.title = "Trading"
        console.log("trading completed")
     }

    id: tradingroot

        // spacer
    Rectangle{width: ProtoScreen.guToPx(.125); height: ProtoScreen.guToPx(1);color: "transparent"}

    Card {
        backgroundColor: "white"
        flat: true
        id: topcard
        radius: 0
        width: parent.width
        height: parent.height
        anchors{
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
//            height: cBan.height - ProtoScreen.guToPx(1)
            width: stack.topdepthleftwidth
            anchors.left: parent.left
            anchors.margins: ProtoScreen.guToPx(1)
            color: "transparent" // Theme.light.textColor//themeroot.theme.secondaryColor
//            radius: 2
            anchors.verticalCenter: cBan.verticalCenter
            visible: liveorresult === "Live"
            Layout.fillHeight: true
            Layout.fillWidth: false

            Label {
                anchors.centerIn: parent
                text: wkorrow + " FANTASY FUTURES";
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                color: "green" //themeroot.theme.primaryColor //liveorresult === "Live" ?  :
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                id: futlabel
                font.bold: true
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: seasontext + " " + liveorresult

            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
            color: themeroot.theme.primaryColor //liveorresult === "Live" ?  :
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            id: cBan
        }

        Label {
            text: "TRADE: "
            color: "green"
            anchors.right: barrec.left
            anchors.margins: ProtoScreen.guToPx(.25)
            font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: barrec.verticalCenter
            font.bold: true
            id: tradeLbl
        }

        Rectangle {
            id: barrec
            height: cBan.height - ProtoScreen.guToPx(1.25)
            width: ProtoScreen.guToPx(22)
            anchors.right: parent.right
            anchors.rightMargin: ProtoScreen.guToPx(2)
//            anchors.left: parent.left
            color:  themeroot.theme.primaryColor // Theme.light.textColor//themeroot.theme.secondaryColor
//            radius: ProtoScreen.guToPx(.5)
            anchors.verticalCenter: cBan.verticalCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "transparent"
            border.width: ProtoScreen.guToPx(.125)
            TabBar {
                selectedIndex: 0
                id: bar
                tabs: ["wk" + MiddleMan.theWeek ,"row"]
                centered: true
//                width: parent.width * 95.0
//                height: parent.height * 95.0
                darkBackground: true
                anchors.centerIn: parent
                anchors.fill: parent
//                anchors.margins: ProtoScreen.guToPx(.125)
                onSelectedIndexChanged: {
                    wkorrow = (tabs[selectedIndex] === "row") ? ("REST-OF-WAY") : ("WEEK-" + MiddleMan.theWeek)
                }
                Layout.fillHeight: false
                Layout.fillWidth: true
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter;
                width: ProtoScreen.guToPx(.125)
                height: parent.height
                color: "white"
                anchors.verticalCenter: parent.verticalCenter

            }
        }

        StackLayout {
            currentIndex: bar.selectedIndex

            anchors.top: cBan.bottom
            anchors.topMargin: ProtoScreen.guToPx(.25)
            width: parent.width
            height: parent.height - cBan.height
            id: stack
            property real topdepthleftwidth: (currentIndex === 0) ? wkTrading.leftwidth : rowTrading.leftwidth

            TradingComp  {
                id: wkTrading
                width: parent.width
                height: parent.height
                quoteitem: MiddleMan.pPlayerQuoteSliceModelItem
                tradepos:   MiddleMan.pTradingPositionsModel
                depthmodel: MiddleMan.pDepthMarketModel
                globalorders: MiddleMan.pGlobalOpenOrdersModel
                quoteproxymodel: MiddleMan.pPlayerQuoteSliceViewFilterProxyModel
                isweekly: true
            }

            TradingComp  {
                id: rowTrading
                width: parent.width
                height: parent.height
                quoteitem: MiddleMan.pROWPlayerQuoteSliceModelItem
                tradepos:   MiddleMan.pROWTradingPositionsModel
                depthmodel: MiddleMan.pROWDepthMarketModel
                globalorders: MiddleMan.pROWGlobalOpenOrdersModel
                quoteproxymodel: MiddleMan.pROWPlayerQuoteSliceViewFilterProxyModel
                isweekly: false
            }

        }
    }

    Connections {
        target: MiddleMan
        onHaveWeeklySymbol: {
            console.log( " onhaveweekly ")
            if (wkTrading.wwww.viewselection.count === 0 )
                wkTrading.wwww.viewselection.count.select(0)
        }
        onHaveRowSymbol: {
            console.log( " onhaverow ")
            if (rowTrading.wwww.viewselection.count === 0 )
                rowTrading.wwww.viewselection.count.select(0)
        }

    }

}

