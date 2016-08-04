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

    Scrollbar{flickableItem: f2}
    Flickable{
        id: fl
        width: parent.width
        height:    parent.height
        contentHeight: parent.height * 2
        contentWidth: parent.width
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        Card{
            id: topcard
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

                width: fl.width / 1.07
//                width: parent.width
                height: ProtoScreen.guToPx(6)
                anchors.bottomMargin:ProtoScreen.guToPx(1)
                anchors.topMargin:ProtoScreen.guToPx(.5)
                anchors{
                    top: parent.top

//                    horizontalCenter: parent.horizontalCenter
                }
            }

//                object:
            ListItems.Subtitled{

                id: listquote
                clip: true
                elevation:  2
//                anchors: parent.width
                backgroundColor:  themeroot.theme.accentColor
                anchors.top: cwc.bottom
                width: parent.width
                text:  "High: " + inplay.hi
                        + " | Low: "+inplay.lo
                        + " | Volume: "+ inplay.volume
//                        + " | OI: " + inplay.asksize
                subText:{
                    "Bid Size: " + inplay.bidsize
                            + " | Bid: "+inplay.bid
                            + " | Ask: "+ inplay.ask
                            + " | Ask Size: " + inplay.asksize
                }
                secondaryItem: RowLayout {
                width: ProtoScreen.guToPx(32)
                height: ProtoScreen.guToPx(8)
                Label{
                    id: las
                    text: "Last: " + inplay.lastprice + "" + arrow.text

                    color: inplay.updown < 0 ? Colors.red :
                             inplay.updown > 0 ? Colors.green : "black"


                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: arrow
                    text: (inplay.updown < 0) ? " ↓" : " ↑";
                    color: "transparent"
                }

                Label{
                        text: inplay.change
                    Layout.fillHeight: true
                    Layout.fillWidth:  false
                    verticalAlignment: Text.AlignVCenter
                }

                Icon{
                    Layout.fillWidth:  false
                    Layout.fillHeight: true
                    hasColor:true
                    color: { inplay.change < 0 ? Colors.red :
                             inplay.change > 0 ? Colors.green : "transparent"
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

            Banner {
                id: bandepth
                anchors.top: listquote.bottom
                anchors.horizontalCenter: listquote.horizontalCenter
    //                height: parent.height - cwc.height - buySell.heigth
                text: "Market Depth"
                    anchrosType: "verticalCenter"
                helperHeader: "Market Depth Help"
                helperTxt: " "
                helpShown: true
                height: ProtoScreen.guToPx(6)
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottomMargin:ProtoScreen.guToPx(.5)
                anchors.topMargin:ProtoScreen.guToPx(.5)
                width: parent.width / 1.10
                anchors.right: parent.right

            }


        Flickable{
            property int depthsize: 5
            property int dihight: ProtoScreen.guToPx(4)
            id: f2
            width: depthvm.width
            height:    depthvm.height
            contentHeight: dihight * Math.max(depthsize,depthvm.count);
            contentWidth: parent.width
            interactive: true
            boundsBehavior: Flickable.StopAtBounds

              ListView {
                id: depthvm
                    anchors.top: bandepth.bottom
                    anchors.margins: 1
                    anchors.horizontalCenter: bandepth.horizontalCenter
                height: ProtoScreen.guToPx(4) * depthsize;
//                    anchors.fill: parent
                    width: bandepth.width
                    height: parent.height - bandepth.height - cwc.height - listquote.height
//                    height: parent.height
                clip: true
                model:  MiddleMan.pDepthMarketModel
                header: RowLayout {
                    width: parent.width
                        height: ProtoScreen.guToPx(4)
                        spacing: 1
                    Rectangle{width: 2; height: 1;color: "transparent"}
                    Repeater{
                        model: ["Bid Size","Bid","Ask","Ask Size"]
                        Card{
                            Layout.fillHeight: true
                                Layout.fillWidth: false
                                Layout.preferredWidth: (parent.width / 4) - 2;                                border.color:"black"
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
                    height: f2.dihight
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
            width: depthvm.width
            anchors.top: depthvm.bottom
//            anchors.bottom: parent.bottom
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: ProtoScreen.guToPx(8)

            Row{
                width: parent.width
                height:parent.height
                Button {
                    elevation: 5
                    text: "BUY"
                    width: parent.width / 2
                    height: width /2
                    onClicked: {
                    }
                }
                Button {
                    elevation: 5
                    text: "SELL"
                    width: parent.width / 2
                    height: width /2
                    onClicked: {
                    }
                }

            }
        }
        }
    }
}


