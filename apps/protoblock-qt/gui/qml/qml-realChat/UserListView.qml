import QtQuick 2.1
import QtQuick.Controls 1.4
import Communi 3.0

import Material 1.0
import Material.ListItems  1.0 as ListItems
import ProtoblockQml.Theme 1.0
Card {
    id: background
    property IrcChannel channel
    signal queried(IrcUser user)
    elevation: 0
    width: parent.width
    height: parent.height
    ScrollView {
        id: scrollView
        anchors.fill: parent
        ListView {
            id: listView
            model: IrcUserModel {
                id: userModel
                sortMethod: Irc.SortByTitle
                channel: background.channel
                onChannelChanged: listView.currentIndex = -1
            }
            delegate: ListItems.Subtitled{
                elevation: 1
                width: parent.width
                text:  model.title
                subText: "Balance: 0"
                action: Image{
                    height: parent.height
                    width : height
                    fillMode: Image.PreserveAspectFit
                    source:  "qrc:/icons/action_account_circle.png"
                }
                onClicked: queried(model.user)
                onPressAndHold: listView.currentIndex = index
            }
        }
    }
}
