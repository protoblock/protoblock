import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
//import ProRotoQml.Models 1.0

Item {
    property string isymbol: MiddleMan.getOrderModelSymbol()
    property string pnamep: MiddleMan.getPlayerNamePos(isymbol)
    property string tid: MiddleMan.getTeamid(isymbol)

//    property variant inplay: MiddleMan.getOrderModel(symbol)

    property int depthsize: 5
    property double dihight: ProtoScreen.guToPx(4)

    Component.onCompleted: {
         pageHelper.title = "Open Orders: " + pnamep

//         pid.txtN = inplay.playerid
    }


         Card{
            id: topcard
            width: parent.width * .90
            height: parent.height
            elevation: 0
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
                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
                bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                text: pnamep + " Open Orders"
                color: "white"
                backgroundColor: themeroot.theme.primaryColor
//                helpShown: true
//                helperHeader: inplay.fullname + " (" + inplay.position + ") Season Contract"
//                helperTxt: "Contract expires after week 15 at the total fantasy points scored by " + inplay.fullname +
//                           "the \"Writer\" (seller) must give the \"buyer\" the equivanlat amount of fantsy points scored in fantasy bits." +
//                           "the game is to try to buy well below the actual settlement number, or alternativly to sell at a price well above the actual final number" +
//                           "with all the know risks, how much are you willing to \"pay\" for the contract, and for how much would you be willing to " +
//                           " write, or sell, the contract, knowing that you keep all the points in cae of injury, but have to pay up in case of a breakout"

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


                ListItems.Subtitled {
                    id: position
                    elevation: 1
                    backgroundColor: themeroot.theme.accentColor

                    width: parent.width
                    text: { pnamep }
//                    subText:{
//                            //"Net Price: " + netprice + " | Net Qty: " + netqty
//                            //  + " | Ask: "+ model.ask
//                            //  + " | Ask Size: " + model.asksize
//                    }

//                    secondaryItem: RowLayout{
//                        width: ProtoScreen.guToPx(32)
//                        height: ProtoScreen.guToPx(8)
//                        Label{
//                            id: pnl
//                            text: "Open Pnl: " + openpnl.toString()

//                            color: "white"

//                            Layout.fillHeight: true
//                            Layout.fillWidth:  true
//                            verticalAlignment: Text.AlignVCenter
//                        }
//                    }

//                    Label{
//                        text: model.avgprice
//                        Layout.fillHeight: true
//                        Layout.fillWidth:  false
//                        verticalAlignment: Text.AlignVCenter
//                    }

//                       valueText: {
//                           ( netqty > 0 ) ? "Long (" + netqty.toString() +")" :
//                           ( netqty < 0 ) ? "Short (" + netqty.toString() +")" :
//                                            "Flat"
//                        }

                }
             }

            Banner {
                id: bandepth
                anchors.top: boundquote.bottom
                anchors.horizontalCenter: boundquote.horizontalCenter
    //                height: parent.height - cwc.height - buySell.heigth
                text: "Open Order"
                anchrosType: "center"
                helperHeader: "Open Orders Help"
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


            Rectangle {
                id: boundingRect
                anchors.top: bandepth.bottom
//                height:    (topcard.height - bandepth.height - boundquote.height - buySell.height )
                height: dihight * ( 1 + Math.min(depthsize,ordersl.count))
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


                    id: ordersl
    //                    anchors.top: bandepth.bottom
                    anchors.margins: 1
                    anchors.horizontalCenter: boundingRect.horizontalCenter
    //                    anchors.fill: parent
                    width: parent.width
    //                height: parent.height - bandepth.height - cwc.height - listquote.height
    //                    height: parent.height
                    clip: true
                    model: MiddleMan.pGlobalOpenOrdersModel
//                    symbol: MiddleMan.pGlobalOpenOrdersModel.pidsymbol
                    headerPositioning: ListView.OverlayHeader
                    header: RowLayout {
                        width: parent.width
                        height: dihight * .75
                        spacing: 1
//                        Rectangle{width: 2; height: 1;color: "transparent"}
                        Repeater{
                            model: ["Cancel","Symbol","Refnum","Side","Price", "Size"]
                            Card{
                                Layout.fillHeight: true
                                Layout.fillWidth: false
                                Layout.preferredWidth: (parent.width / 6) - 2;
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

                    delegate: OMSCard {
                        height: dihight
                        elevation: 2
                        width: parent.width
                        refnum:  model.refnum
                        qty: model.size
                        price: model.price
                        side: model.isbuy ? 1 : -1
                        symbol: isymbol

                }
                }
            }
        }



//         Connections {
//             target: MiddleMan
//             onPPlayerQuoteSliceModelItemChanged: {
//                 ordersl.model = MiddleMan.pPlayerQuoteSliceModelItem.ordersModel
//             }
//         }
}


