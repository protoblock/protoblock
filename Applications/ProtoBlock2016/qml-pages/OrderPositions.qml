import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems

import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Models 1.0


Item {
    height: parent.height
    width: parent.width
    Component.onCompleted: {
         pageHelper.title = "Orders Positions " + realRoot.uname

        if ( !realRoot.reloadorderpos )
            realRoot.reloadorderpos = true
        else {
            MiddleMan.getOrderPos()
            realRoot.reloadorderpos = false
        }
    }

    property int orderlistsize: 5
    property double dihight: ProtoScreen.guToPx(4)
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
        elevation: 1
//            anchors.centerIn: parent
        anchors{
            topMargin:ProtoScreen.guToPx(.5)
//            horizontalCenter: parent.horizontalCenter
            margins: ProtoScreen.guToPx(1)
        }

        // spacer
        Rectangle{width: 1; height: ProtoScreen.guToPx(1);color: "transparent"}

        Banner {
            id: mybanner
//                Layout.fillWidth: true;
//                anchrosType: "center"
            fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
            bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Portfolio for: " +  MiddleMan.pTradingPositionsModel.fantasyname
            color: "white"
            backgroundColor: themeroot.theme.primaryColor
            helpShown: true
            helperHeader: "Orders and Positions"
            helperTxt: "jay@protoblock.com for help"
            height: ProtoScreen.guToPx(6)
            anchors.bottomMargin:ProtoScreen.guToPx(.5)
            anchors.topMargin:ProtoScreen.guToPx(.5)
            anchors.top: parent.top
            //              horizontalCenter: parent.horizontalCenter
        }

            ListView{
               id: poslistview
               anchors.margins: 1
               width: topcard.width
               height: topcard.height - mybanner.height
//                id: playersListView
               anchors.top: mybanner.bottom
//               anchors.fill: parent

               model: MiddleMan.pTradingPositionsModel
               delegate: Item {
                   anchors.top: parent.top
                   height: ProtoScreen.guToPx(16)
                   width: parent.width
                   anchors.verticalCenter: parent.verticalCenter

                   property string tid: MiddleMan.getTeamid(symbol)
                   property string disp:  MiddleMan.getPlayerNamePos(symbol)

                    ListItems.Subtitled {

                        id: position
                        elevation: 1
                        backgroundColor: "white"
                        width: parent.width
                        height: parent.height
                        anchors.top: parent.top
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

                                color: model.openpnl < 0 ? Colors.red :
                                         model.openpnl > 0 ? Colors.green : "black"

                                Layout.fillHeight: true
                                Layout.fillWidth:  true
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        action: Icon{
                            hasColor:false
                            source: "qrc:/"+ tid  + ".PNG"
                            width: ProtoScreen.guToPx(6)
                            height: width
                            smooth: true
                        }
                        onClicked: {}

                    }

                    RowLayout {
                        id: r2
                        anchors.top: position.bottom
                        width: parent.width
                        height: dihight * .75
                        spacing: 1
        //                        Rectangle{width: 2; height: 1;color: "transparent"}
                        Repeater{
                            model: ["x","Symbol","Refnum","Side","Price", "Size"]
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

                    ColumnLayout {
                        anchors.top: r2.bottom
//                        height: dihight * pOpenOrdersModel.count
                        width: parent.width
                        Repeater{
                           model: pOpenOrdersModel
//                           height: parent.height
//                           anchors.top: parent.top
                            OMSCard {
                            height: dihight
                            elevation: 2
                            width: parent.width
                            refnum:  model.refnum
                            qty: model.size
                            price: model.price
                            side: model.isbuy
                            symbol: model.symbol
                        }

                    }

                }

//                Scrollbar{flickableItem: ordersListView }

             }

        }

}
}
}
