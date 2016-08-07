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
         pageHelper.title = "Orders Positions " + realRoot.uname

        if ( !realRoot.reloadorderpos )
            realRoot.reloadorderpos = true
        else {
            MiddleMan.getOrderPos()
            realRoot.reloadorderpos = false
        }

//        if ( realRoot.reloadrowquote ) {
//            MiddleMan.rowMarketGet()
//            realRoot.reloadrowquote = false
//        }
    }

    Scrollbar{flickableItem: fl}
    Flickable{
        id: fl
        width: parent.width
        height:    parent.height
        contentHeight: parent.height * 5
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
                id: mybanner
    //                Layout.fillWidth: true;
    //                anchrosType: "center"
                anchors.horizontalCenter: parent.horizontalCenter
                bold: true
                text: "Portfolio for: " +  MiddleMan.pTradingPositionsModel.fantasyname
                color: "white"
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: "Orders and Positions"
                helperTxt: "jay@protoblock.com for help"
                width: fl.width / 1.07
                height: ProtoScreen.guToPx(6)
                anchors.bottomMargin:ProtoScreen.guToPx(.5)
                anchors.topMargin:ProtoScreen.guToPx(.5)
                anchors.top: parent.top
            }

            ListView{
                anchors.margins: 1
                width: topcard.width
                height: topcard.height - ProtoScreen.guToPx(6)
                id: poslistview
                anchors.top: mybanner.bottom
                model: MiddleMan.pTradingPositionsModel
                delegate:
                    ListItems.Subtitled {
                        property string tid: MiddleMan.getTeamid(symbol)
                        property string disp:  MiddleMan.getPlayerNamePos(symbol)

                        id: position
                        elevation: 1
                        backgroundColor: model.openpnl < 0 ? Colors.red :
                                        model.openpnl > 0 ? Colors.green : "white"
                        width: parent.width
    //                    Layout.fillHeight: false
                        text: { disp }
                        subText:{ "Net Price: " + netprice + " | Net Qty: " + netqty
                                //  + " | Ask: "+ model.ask
                                //  + " | Ask Size: " + model.asksize
                        }

                        secondaryItem: RowLayout{
                            width: ProtoScreen.guToPx(32)
                            height: ProtoScreen.guToPx(8)
                            Label{
                                id: pnl
                                text: "Open Pnl: " + openpnl.toString()

                                color: "white"
//                                color: model.openpnl < 0 ? Colors.red :
//                                         model.openpnl > 0 ? Colors.green : "black"

                                Layout.fillHeight: true
                                Layout.fillWidth:  true
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Label{
                            text: model.avgprice
                            Layout.fillHeight: true
                            Layout.fillWidth:  false
                            verticalAlignment: Text.AlignVCenter
                        }

                       valueText: {
                           ( netqty > 0 ) ? "Long (" + netqty.toString() +")" :
                           ( netqty < 0 ) ? "Short (" + netqty.toString() +")" :
                                            "Flat"
                        }

                        action: Icon {
//                            visible: tid != "" && tid != undefined
                            hasColor:false
                            source: "qrc:/"+ tid  + ".PNG"
                            width: ProtoScreen.guToPx(6)
                            height: width
                            smooth: true
                        }
                        onClicked: {
                            MiddleMan.setOrderModel(symbol)
                            rootLoader.source = "qrc:/Orders.qml"
                        }

                    }
            }
            Scrollbar{flickableItem: poslistview }
        }
    }
}



//fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
