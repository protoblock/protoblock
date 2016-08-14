import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
//import ProRotoQml.Models 1.0

Item {

    id: pit
    property string contract
    property string symbol
    property variant inplay: MiddleMan.pDepthMarketModel.pPlayerQuoteSliceModelItem


    property int depthsize: 5
    property double dihight: ProtoScreen.guToPx(4)
    property int price
    property bool isppgslider: false

    Component.onCompleted: {
         symbol = inplay.symbol
         pageHelper.title = "Trading " + symbol

//         pid.txtN = inplay.playerid
//        if ( !realRoot.reloadrowquote )
//            realRoot.reloadrowquote = true
//        else {
//            MiddleMan.rowMarketGet()
//            realRoot.reloadrowquote = false
//        }
    }

    Component.onDestruction: {
        MiddleMan.stopDepth(symbol)
    }

    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 3
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
//    Scrollbar{flickableItem: f1}
//    Flickable{
//        id: fl
//        width: parent.width
//        height:    parent.height
//        contentHeight: parent.height * 2
//        contentWidth: parent.width
//        interactive: true
//        boundsBehavior: Flickable.StopAtBounds

        Card{
            id: topcard
            width: parent.width * .90
            height: parent.height
            elevation: 0
//            anchors.centerIn: parent
            anchors{
                top: parent.top
                topMargin:ProtoScreen.guToPx(.5)
                horizontalCenter: parent.horizontalCenter
                margins: ProtoScreen.guToPx(.125)
            }

            // spacer
            Rectangle{width: ProtoScreen.guToPx(.125); height: ProtoScreen.guToPx(1);color: "transparent"}

            Banner {
                id: cwc
//                Layout.fillWidth: true;
//                anchrosType: "center"
                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
                bold: true
                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.horizontalCenterOffset: ppgslider.
                text: inplay.fullname + " (" + inplay.position +")" +
                    " 2016 Season Trading (0/16)"
                color: "white"
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: inplay.fullname + " (" + inplay.position + ") Season Contract"
                helperTxt: "16 games 16 weeks. Season long contracts settle at the total points from a 16 game season. " +
                            "the season is from week1-week16. (week 16 is counted twice instead of using week 17)." +
                            "These expire at the total fantasy points scored by " + inplay.fullname +
                            "the \"Writer\" (seller) must give the \"buyer\" the equivanlat amount of fantsy points scored in fantasy bits." +
                            "It is 1 Fantasy Bit pe Fantaasy Point in these season long contracts" +
                            "the game is to try to buy well below the actual settlement number," +
                            "or alternativly to write (sell) at a price well above the actual final number" +
                            "with all the know risks, how much are you willing to \"pay\" for the contract," +
                            " and for how much would you be willing to " +
                            " write, or sell, the contract, knowing that you keep all the points in cae of injury," +
                            " but have to pay up in case of a breakout?"

                width: parent.width / 1.10
//                width: parent.width
//                height: ProtoScreen.guToPx(6)
                anchors.bottomMargin:ProtoScreen.guToPx(1)
                anchors.topMargin:ProtoScreen.guToPx(.5)
                anchors{
                    top: parent.top

//                    horizontalCenter: parent.horizontalCenter
                }
            }

    //                object:


            Rectangle {
                id: boundquote
                width: parent.width / 1.10
                height: ProtoScreen.guToPx(8)
                anchors{
                    top: cwc.bottom

                    horizontalCenter: cwc.horizontalCenter
                }


                ListItems.Subtitled{
                    anchors.fill: parent
                    id: listquote
                    clip: false
                    elevation:  2
    //                anchors.horizontalCenter: cwc.horizontalCenter
    //                anchors.horizontalCenter: topcard.horizontalCenter
                    backgroundColor:  themeroot.theme.accentColor

                    width: parent.width
                    subText: {   "Vol: "+ inplay.volume
                            + " | Hi: " + inplay.hi
                            + " | Lo: "+inplay.lo

    //                        + " | OI: " + inplay.asksize
                    }

                    text: {
//                        "Bid Size: " + inplay.bidsize
                                "Bid: "+ inplay.bidsize + " @ " +inplay.bid
                                + " | Ask: "+ inplay.asksize + " @ " + inplay.ask
//                                + " | Last: "+ inplay.lastprice + " " + arrow.text

//                                + " | Ask Size: " + inplay.asksize

                    }
                    secondaryItem: RowLayout {
//                        width: ProtoScreen.guToPx(32)
                        layoutDirection: "LeftToRight"
                        height: ProtoScreen.guToPx(8)


                        Label{
                            id: las
                            text: "Last: " + inplay.lastprice.toString()

                            Layout.fillHeight: true
                            Layout.fillWidth:  false
                            verticalAlignment: Text.AlignVCenter
                            color: "black"
//                            horizontalAlignment: Text.Right
//                            anchors.left: parent.left
                        }

                        Text {
                            id: arrow
                            text: (inplay.updown < 0) ? "↓ " : (inplay.updown > 0) ? "↑ " : " ";
//                            color: "transparent"
                            color: inplay.updown < 0 ? Colors.red :
                                     inplay.updown > 0 ? Colors.green : "transparent"
                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.Right

                            anchors.left: las.right;
                        }


                        Label{
                            text: "Change: "
                            Layout.fillHeight: true
                            Layout.fillWidth:  false
                            verticalAlignment: Text.AlignVCenter
                            anchors.right: change.left
                            horizontalAlignment: Text.Right
                        }

                        Label{
                            id: change
                            text: ((inplay.change > 0) ? "+" : "" ) + inplay.change.toString() + " "
                            Layout.fillHeight: true
                            Layout.fillWidth:  false
                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.Right
                            color: icon.color
                        }

                        Icon{
                            id: icon
//                            anchors.left: change.right
                            Layout.fillWidth:  false
                            Layout.fillHeight: true
                            hasColor:true
                            color: { inplay.change < 0 ? Colors.red :
                                     inplay.change > 0 ? Colors.green : Colors.black
                            }
                            source: {
                                if (inplay.change < 0 ){
                                    "qrc:/icons/ic_trending_down.png"

                                    ////ic_trending_down.png"

                                }
                                else
                                {
                                    if (inplay.change === 0 )
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
                        id: teamicon
                        hasColor:false
                        source: "qrc:/"+ inplay.teamid+".PNG"
                        width: ProtoScreen.guToPx(6)
                        height: width
                    }
                }
            }

            Banner {
                id: bandepth
                anchors.top: boundquote.bottom
                anchors.horizontalCenter: boundquote.horizontalCenter
    //                height: parent.height - cwc.height - buySell.heigth
                text: "Market Depth"
                anchrosType: "top"
                anchrosHType: "center"
                helperHeader: "Market Depth Help"
                helperTxt: " "
                helpShown: true
                height: ProtoScreen.guToPx(6)
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottomMargin:ProtoScreen.guToPx(.5)
                anchors.topMargin:ProtoScreen.guToPx(.5)
                width: parent.width * .75
                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
                bold: true
//                anchors.right: parent.right

            }
/*
            Slider {
                id: gamesslider
                anchors.bottom: boundingRect.top
                anchors.horizontalCenter: bandepth.horizontalCenter
                width: bandepth.width / 1.20
                numericValueLabel: true
//                alwaysShowValueLabel: true
                tickmarksEnabled: true
                minimumValue: 0
                value: 16
                maximumValue: 16
                stepSize: 1
                knobLabel: value + " Games"
                knobDiameter: ProtoScreen.guToPx(6)
                darkBackground: false
                color: "white"
                onPressedChanged: {
//                    if ( pressed && isppgslider ) {
//                        isppgslider = false;
//                        ppgslider.value = 1;
//                    }
                    if ( pressed )
                        ppgslider.forceActiveFocus();
                    else
                        forceActiveFocus()
                }
//                onValueChanged: {
//                    if ( focus )
//                        ppgslider.forceActiveFocus();

//                }

                activeFocusOnPress: true
//                onHoveredChanged: {
//                    if ( hovered && !focus )
//                        forceActiveFocus();
//                }
            }


            Slider {
                id: ppgslider
                orientation: Qt.Vertical
                anchors.top: boundquote.bottom
                anchors.right: bandepth.left
                anchors.rightMargin: ProtoScreen.guToPx(2)
                anchors.left: boundquote.left
                height: boundquote.height + boundingRect.height
                width: (boundquote.width - bandepth.width) / 2
//                anchors.horizontalCenter: teamicon.horizontalCenter
                numericValueLabel: true
//                alwaysShowValueLabel: true
                tickmarksEnabled: true
                minimumValue: 1
                value: 10
                maximumValue: 40
                stepSize: 1
                knobLabel: value + " PPG"
                knobDiameter: ProtoScreen.guToPx(6)
//                darkBackground: false
//                color: "white"
                onPressedChanged: {
//                    if ( pressed && !isppgslider ) {
//                        isppgslider = true;
//                        gamesslider.value = 0;
//                    }
                    if ( pressed )
                        gamesslider.forceActiveFocus()
                    else
                        forceActiveFocus()
                }
                activeFocusOnPress: true
            }

*/
//            Scrollbar{flickableItem: f1}

//            Flickable{
//                anchors.top: bandepth.bottom

//                property int depthsize: dihight
//                property int dihight: ProtoScreen.guToPx(4)
//                id: f2
//                width: depthvm.width
//                height:    (parent.height - cwc.height - listquote.height - buySell.height )
////                dihight * Math.max(depthsize,depthvm.count);
//                contentHeight: dihight * depthvm.count;
//                contentWidth: parent.width
//                interactive: true
//                boundsBehavior: Flickable.StopAtBounds

            Rectangle {
                id: boundingRect
                anchors.top: bandepth.bottom
//                anchors.topMargin: ProtoScreen.guToPx(1)
//                height:    (topcard.height - bandepth.height - boundquote.height - buySell.height )
                height: dihight * ( .75 + Math.min(depthsize,depthvm.count))
//                anchors.margins: ProtoScreen.guToPx(.125)
                anchors.horizontalCenter: bandepth.horizontalCenter
                width: bandepth.width
                color: "transparent"
                Rectangle {
                    id: middlebar
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    color: "grey"
                    width: ProtoScreen.guToPx(2)
                    border.color: "black"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                ListView {
                    anchors.fill: parent

    //                    id: f2
    //                    width: depthvm.width
                    height: parent.height
    //                    (parent.height - cwc.height - listquote.height - buySell.height )
    //                dihight * Math.max(depthsize,depthvm.count);
    //                    contentHeight: dihight * count;
    //                    contentWidth: bandepth.width
    //                    interactive: true
    //                    boundsBehavior: Flickable.StopAtBounds


                    id: depthvm
    //                    anchors.top: bandepth.bottom
//                    anchors.margins: ProtoScreen.guToPx(.125)
                    anchors.horizontalCenter: boundingRect.horizontalCenter
    //                    anchors.fill: parent
                    width: boundingRect.width
    //                height: parent.height - bandepth.height - cwc.height - listquote.height
    //                    height: parent.height
                    clip: true
                    model: MiddleMan.pDepthMarketModel
                    headerPositioning: ListView.OverlayHeader
                    header: Item {
                        height: leftrow.height
                        width: parent.width
                        RowLayout {
                            id: leftrow
                            width: ((parent.width - middlebar.width)/2.0)  + 1
                            height: dihight * .75
                            spacing: 0 //ProtoScreen.guToPx(.125)
//                            anchors.left:parent.left
                            anchors.right: parent.horizontalCenter
                            anchors.rightMargin: (middlebar.width / 2.0) - .5
//                            anchors.right: rightrow.left
//                            anchors.rightMargin: middlebar.width
//                            anchors.rightMargin: middlebar.width / 2
    //                        Rectangle{width: 2; height: 1;color: "transparent"}

                            Card{
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.preferredWidth: (parent.width / 5.0) //- ProtoScreen.guToPx(.125) * 2
                                border.color:"black"
                                backgroundColor: "grey"
                                Label{
                                    anchors.centerIn: parent
                                    text: "PPG"
                                    font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                                    color: "white"
                                }
                            }
                            Repeater{
                                model: ["Qty","Bid"]
                                Card{
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: ((parent.width / 5.0) * 2.0) + .5 //- ProtoScreen.guToPx(.125) * 2
                                    border.color:"black"
                                    backgroundColor: Colors.blue
                                    Label{
                                        anchors.centerIn: parent
                                        text: modelData
                                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                        color: "white"
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }




                        }

                        RowLayout {
                            id: rightrow
                            anchors.right:parent.right
//                            anchors.left: leftrow.right
//                            anchors.leftMargin: middlebar.width
                            width: ((parent.width - middlebar.width)/2.0) + .5
                            height: dihight * .75
                            spacing: 0
//                            anchors.right:parent.right

                            Repeater{
//                                model: ["Ask","Qty","PPG"]
                                model: ["Ask","Qty",]
                                Card{
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: (parent.width / 5.0) * 2.0// - 2)
                                    border.color:"black"
                                    backgroundColor: Colors.blue
                                    Label{
                                        anchors.centerIn: parent
                                        text: modelData
                                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                        color: "white"
                                    }
                                }
                            }

                            Card{
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.preferredWidth: (parent.width / 5.0) // - 2)
                                border.color:"black"
                                backgroundColor: "grey"
                                Label{
                                    anchors.centerIn: parent
                                    text: "PPG"
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

                    delegate: Item {
                            width: boundingRect.width
                            height: dihight
                            MarketDepthCard {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                id: biddepth
//                                width: parent.width // ((parent.width - middlebar.width)/2)
                                anchors.left:parent.left
                                width: ((parent.width - middlebar.width)/2.0) + 1
                                numgames: buycard.numgames
//                                maxnumgames: gamesslider.value
                                numppg: buycard.avgppg
//                                maxppg: ppgslider.value
                                height: dihight
                                elevation: 2
                                buySize:  model.bidsize
                                buy: model.bid
                            }
                            MarketDepthCardAsk {
                                id: askdepth
        //                                width: parent.width // ((parent.width - middlebar.width)/2)
                                anchors.right:parent.right
                                width: ((parent.width - middlebar.width)/2.0)
                                numgames: buycard.numgames
                                numppg: buycard.avgppg
                                height: dihight
                                elevation: 2
                                sellSize: model.asksize
                                sell: model.ask

                            }
                        }
                   // }
                }
            }

            Item {
                id: row3
                width: boundingRect.width * .85
                height: parent.height - boundingRect.height - bandepth.height - boundquote.height - cwc.height
                anchors.top: boundingRect.bottom
                anchors.topMargin: ProtoScreen.guToPx(1)
                anchors.horizontalCenter: boundingRect.horizontalCenter
//                anchors.rightMargin: width * .10

                Item {
                    height: ProtoScreen.guToPx(14)
                    id: bcardrow;
                    width: parent.width
//                    height: parent.height
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter

                        BuyTrading {
                            symbol: symbol
                            id: buycard
    //                        width: parent.width
    //                        height: parent.height
                            calcprice: inplay.lastprice
    //                        onCalculate:

                            Component.onCompleted: {
                                buycard.calculate.connect(doCalc)
                            }

                            function doCalc() {
                                MiddleMan.doDepth();                                                }
                        }
                    }


                    Item {
                        anchors.top: bcardrow.bottom
                        id: row1
                        width: (parent.width / 3.0) * 2.0
                        height: ProtoScreen.guToPx(14)//parent.height / 2
//                        anchors.top: bcardrow.right
                        anchors.margins: ProtoScreen.guToPx(.5)
                        anchors.horizontalCenter: parent.horizontalCenter


                        Rectangle {
                            color: "transparent"
                            width: parent.width / 2.0
//                            height: parent.height - ProtoScreen.guToPx(2)
//                            radius: ProtoScreen.guToPx(1)
//                            anchors.right: parent.right
                            height: parent.height - ProtoScreen.guToPx(2)
                            radius: ProtoScreen.guToPx(1)
                            anchors.right: parent.horizontalCenter


                            Button {
    //                            anchors.fill: parent
                                width: parent.width / 2.0 // ProtoScreen.guToPx(4)
                                height: width / 2.0
    //                            anchors.rightMargin: ProtoScreen.guToPx(1)
    //                            anchors.right: parent.right
    //                            anchors.verticalCenter: parent.verticalCenter
                                anchors.centerIn: parent
    //                            anchors.margins: ProtoScreen.guToPx(.25)
                                text: "BUY"
                                backgroundColor:  Colors.green
                                textColor: "white"
                                onClicked : {
                                    focus = true;
//                                    console.log(" price " + pint.txtN )
                                     MiddleMan.doTrade(
                                            inplay.symbol
                                            ,true
                                         ,buycard.calcprice
                                         ,buycard.qty
                                            )


                                }
                            }
                        }

//                        Text {
//                            id: ttt
//                            text: "a"
//                            Component.onCompleted: {
//                                console.log(" implic width " + ttt.implicitWidth)
//                                console.log(" implic height " + ttt.implicitHeight)

//                            }
//                        }

                        Rectangle {
                            color: "transparent"
                            width: parent.width / 2
                            height: parent.height - ProtoScreen.guToPx(2)
                            radius: ProtoScreen.guToPx(1)
                            anchors.left: parent.horizontalCenter
                            Button {
                                width: parent.width / 2 // ProtoScreen.guToPx(4)
                                height: width / 2.0
    //                            anchors.leftMargin: ProtoScreen.guToPx(1)
    //                            anchors.left: parent.left
    //                            anchors.verticalCenter: parent.verticalCenter
                                anchors.centerIn: parent

    //                            anchors.margins: ProtoScreen.guToPx(.25)
                                text: "SELL"
                                backgroundColor:  Colors.red
                                textColor: "white"
                                onClicked : {
                                    focus = true;

//                                    console.log(" price " + pint.txtN )
                                     MiddleMan.doTrade(
                                            inplay.symbol
                                            ,false
                                         ,buycard.calcprice
                                         ,buycard.qty
                                            )

                                }
                             }

                         }

                    }

//                    BuyTrading {
//                        id: sellCard
//                        width: parent.width / 2
//                        height: parent.height
//                        isbuyCard: false

//                    }
            }

//            Card {
//                id: buySell
//                width: boundingRect.width
//                anchors.top: row3.bottom
////                Layout.fillHeight: true
////                Layout.fillWidth: true
//                height: ProtoScreen.guToPx(12)
//                anchors.topMargin: ProtoScreen.guToPx(.25)
//                anchors {
//                    horizontalCenter: boundingRect.horizontalCenter
////                    margins: ProtoScreen.guToPx(.25)
//                }

//                elevation: 0
//                Row {
////                    spacing: 2
//                    id: row1
//                    width: (parent.width / 3) * 2
//                    height: ProtoScreen.guToPx(4)
//                    anchors.centerIn: parent
//                    anchors.fill: parent
//                    anchors.margins: ProtoScreen.guToPx(.5)
////                    anchors.horizontalCenter: parent.horizontalCenter

//                    Rectangle {
//                        color: "transparent"
//                        width: parent.width / 2
//                        height: parent.height - ProtoScreen.guToPx(2)
//                        radius: ProtoScreen.guToPx(1)
//                        anchors.verticalCenter: parent.verticalCenter

//                        Button {
////                            anchors.fill: parent
//                            width: parent.width / 2 // ProtoScreen.guToPx(4)
//                            height: width / 2.7
////                            anchors.rightMargin: ProtoScreen.guToPx(1)
////                            anchors.right: parent.right
////                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.centerIn: parent
////                            anchors.margins: ProtoScreen.guToPx(.25)
//                            text: "BUY"
//                            backgroundColor:  Colors.green
//                            textColor: "white"
//                            onClicked : {
//                                console.log(" price " + pint.txtN )
//                                 MiddleMan.doTrade(
//                                        inplay.symbol
//                                        ,true
//                                        ,pint.txtN
//                                        ,qint.txtN
//                                        )
//                            }
//                        }
//                    }

//                    Rectangle {
//                        color: "transparent"
//                        width: parent.width / 2
//                        height: parent.height - ProtoScreen.guToPx(2)
//                        radius: ProtoScreen.guToPx(1)
//                        anchors.verticalCenter: parent.verticalCenter
//                        Button {
//                            width: parent.width / 2 // ProtoScreen.guToPx(4)
//                            height: width / 2.7
////                            anchors.leftMargin: ProtoScreen.guToPx(1)
////                            anchors.left: parent.left
////                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.centerIn: parent

////                            anchors.margins: ProtoScreen.guToPx(.25)
//                            text: "SELL"
//                            backgroundColor:  Colors.red
//                            textColor: "white"
//                            onClicked : {
//                                console.log(" price " + pint.txtN )
//                                 MiddleMan.doTrade(
//                                        inplay.symbol
////                                        pid.txtN
//                                        ,false
//                                        ,pint.txtN
//                                        ,qint.txtN
//                                        )

//                            }
//                         }

//                     }

//                }
//            }


//            Card {
//                id: inputs
//                width: buySell.width
//                anchors.top: buySell.bottom
////                Layout.fillHeight: true
////                Layout.fillWidth: true
//                height: ProtoScreen.guToPx(8)
//                anchors.topMargin: ProtoScreen.guToPx(.25)
//                anchors {
//                    horizontalCenter: buySell.horizontalCenter
////                    margins: ProtoScreen.guToPx(.25)
//                }
//                elevation: 0

////                Row {
////                    width: (pint.width + qint.width) * 1.20
////                    height: ProtoScreen.guToPx(4)
////                    anchors.centerIn: parent
////                    anchors.fill: parent
////                    anchors.margins: ProtoScreen.guToPx(.5)
////                    anchors.horizontalCenter: parent.horizontalCenter

////                    id: row2

//                    IntHelper {
//                        id: pint
////                            width: parent.width / 2
//                        labelTxt: "Price"
//                        lo: 1
//                        hi: 400
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.right: parent.horizontalCenter
//                        anchors.rightMargin:ProtoScreen.guToPx(1)
////                        helpShown: true

//                    }
//                    IntHelper {
//                        id: qint
////                            width: parent.width / 2
//                        labelTxt: "Qty"
//                        lo: 1
//                        hi: 100
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.left: pint.right
//                        anchors.leftMargin: ProtoScreen.guToPx(2)
////                        helpShown: true
////                        txtN: gamesslider.value * ppgslider.value
//                    }

////                    IntHelper {
////                        id: pid
////                        labelTxt: "oid"
////                        lo: 1
////                        hi: 10000
////                        anchors.verticalCenter: parent.verticalCenter
////                        anchors.right: pint.left
////                        anchors.rightMargin: ProtoScreen.guToPx(2)
//////                        txtN: inplay.playerid
////                        onChanged: {
////                            MiddleMan.doCancel(txtN);
////                        }
////                    }


//            }

//                Row {
//                    id: row2
//                    anchors.top: row1.bottom
//                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//                    Layout.fillHeight: true
//                    Layout.fillWidth: true

//                    Label{
//                        style: "menu"
//                        id: qytCombotxt
//                        horizontalAlignment: Text.AlignRight
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                        text: "Qty: "
//                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
//                    }
//                    TextField {
//                        anchors.left: qytCombotxt.right
//                        id: qytCombo
//                        validator: IntValidator {bottom: 1; top: 999;}
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    }
//                }

//                Row {
//                    anchors.top: row2.bottom
//                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//                    Layout.fillHeight: true
//                    Layout.fillWidth: true

//                    Label{
//                        id: priceCombotxt
//                        text: " Price: "
//                        horizontalAlignment: Text.AlignRight
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)

//                    }
//                    TextField {
//                        anchors.left: priceCombotxt.right
//                        id: priceCombo
//                        validator: IntValidator {bottom: 1; top: 400;}
//                        text: "1"
////                        onAccepted: {
////                            price = parseInt(text,10)
////                            avgpoints = (1.0 * price) / (1.0 * (numweeks.currentIndex >0 ? numweeks.currentIndex : 15))
////                            avgPoints.currentIndex = avgpoints < 1 ? 1 : avgpoints
////                        }
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    }
//                }

//            }

        }
    }



}
