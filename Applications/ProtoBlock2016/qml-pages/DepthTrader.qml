import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Models 1.0

Item {
    property string contract
    property string symbol
    property variant inplay: MiddleMan.pDepthMarketModel.pPlayerQuoteSliceModelItem

    property int depthsize: 10
    property double dihight: ProtoScreen.guToPx(4)

    Component.onCompleted: {
         pageHelper.title = "Trading " + symbol

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
                margins: 1
            }

            // spacer
            Rectangle{width: 1; height: ProtoScreen.guToPx(1);color: "transparent"}

            Banner {
                id: cwc
//                Layout.fillWidth: true;
//                anchrosType: "center"
                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
                bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                text: inplay.fullname + " (" + inplay.position +")" +
                    " 2016 Season Trading (12/27)"
                color: "white"
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: inplay.fullname + " (" + inplay.position + ") Season Contract"
                helperTxt: "Contract expires after week 15 at the total fantasy points scored by " + inplay.fullname +
                           "the \"Writer\" (seller) must give the \"buyer\" the equivanlat amount of fantsy points scored in fantasy bits." +
                           "the game is to try to buy well below the actual settlement number, or alternativly to sell at a price well above the actual final number" +
                           "with all the know risks, how much are you willing to \"pay\" for the contract, and for how much would you be willing to " +
                           " write, or sell, the contract, knowing that you keep all the points in cae of injury, but have to pay up in case of a breakout"

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
                        hasColor:false
                        source: "qrc:/"+ inplay.team_id+".PNG"
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
                anchrosType: "center"
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
//                height:    (topcard.height - bandepth.height - boundquote.height - buySell.height )
                height: dihight * ( 1 + Math.min(depthsize,depthvm.count))
                anchors.margins: 1
                anchors.horizontalCenter: bandepth.horizontalCenter
                width: bandepth.width

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
                    anchors.margins: 1
                    anchors.horizontalCenter: boundingRect.horizontalCenter
    //                    anchors.fill: parent
                    width: parent.width
    //                height: parent.height - bandepth.height - cwc.height - listquote.height
    //                    height: parent.height
                    clip: true
                    model: MiddleMan.pDepthMarketModel
                    headerPositioning: ListView.OverlayHeader
                    header: RowLayout {
                        width: parent.width
                        height: dihight * .75
                        spacing: 1
//                        Rectangle{width: 2; height: 1;color: "transparent"}
                        Repeater{
                            model: ["Bid Size","Bid","Ask","Ask Size"]
                            Card{
                                Layout.fillHeight: true
                                Layout.fillWidth: false
                                Layout.preferredWidth: (parent.width / 4) - 2;
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
                    delegate: MarketDepthCard {
                        height: dihight
                        elevation: 2
                        width: parent.width
                        buySize:  model.bidsize
                        buy: model.bid
                        sellSize: model.asksize
                        sell: model.ask
                    }
                }
            }
            Card {
                id: buySell
                width: boundingRect.width
                anchors.top: boundingRect.bottom
//                Layout.fillHeight: true
//                Layout.fillWidth: true
                height: ProtoScreen.guToPx(12)
                anchors.topMargin: ProtoScreen.guToPx(.25)
                anchors {
                    horizontalCenter: boundingRect.horizontalCenter
//                    margins: ProtoScreen.guToPx(.25)
                }

                elevation: 0
                Row {
//                    spacing: 2
                    id: row1
                    width: (parent.width / 3) * 2
                    height: ProtoScreen.guToPx(4)
                    anchors.centerIn: parent
                    anchors.fill: parent
                    anchors.margins: ProtoScreen.guToPx(.5)
//                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        color: "transparent"
                        width: parent.width / 2
                        height: parent.height - ProtoScreen.guToPx(2)
                        radius: ProtoScreen.guToPx(1)
                        anchors.verticalCenter: parent.verticalCenter
                        Button {
//                            anchors.fill: parent
                            width: parent.width / 2 // ProtoScreen.guToPx(4)
                            height: width / 2.7
//                            anchors.rightMargin: ProtoScreen.guToPx(1)
//                            anchors.right: parent.right
//                            anchors.verticalCenter: parent.verticalCenter
                            anchors.centerIn: parent
//                            anchors.margins: ProtoScreen.guToPx(.25)
                            text: "BUY"
                            backgroundColor:  Colors.green
                            textColor: "white"
                            onClicked: {
                            }
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        width: parent.width / 2
                        height: parent.height - ProtoScreen.guToPx(2)
                        radius: ProtoScreen.guToPx(1)
                        anchors.verticalCenter: parent.verticalCenter
                        Button {
                            width: parent.width / 2 // ProtoScreen.guToPx(4)
                            height: width / 2.7
//                            anchors.leftMargin: ProtoScreen.guToPx(1)
//                            anchors.left: parent.left
//                            anchors.verticalCenter: parent.verticalCenter
                            anchors.centerIn: parent

//                            anchors.margins: ProtoScreen.guToPx(.25)
                            text: "SELL"
                            backgroundColor:  Colors.red
                            textColor: "white"
                            onClicked: {
                            }
                        }
                    }

                }
                Row {
                    id: row2
                    anchors.top: row1.bottom
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label{
                        style: "menu"
                        id: qytCombotxt
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                        text: "Qty: "
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }
                    TextField {
                        anchors.left: qytCombotxt.right
                        id: qytCombo
                        validator: IntValidator {bottom: 1; top: 999;}
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                    }
                }

                Row {
                    anchors.top: row2.bottom
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label{
                        id: priceCombotxt
                        text: " Price: "
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)

                    }
                    TextField {
                        anchors.left: priceCombotxt.right
                        id: priceCombo
                        validator: IntValidator {bottom: 1; top: 400;}
                        text: "1"
//                        onAccepted: {
//                            price = parseInt(text,10)
//                            avgpoints = (1.0 * price) / (1.0 * (numweeks.currentIndex >0 ? numweeks.currentIndex : 15))
//                            avgPoints.currentIndex = avgpoints < 1 ? 1 : avgpoints
//                        }
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                    }
                }

            }

        }
    }



