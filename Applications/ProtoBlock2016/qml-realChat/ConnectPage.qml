import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import Qt.labs.settings 1.0
import Communi 3.0

Item {
    id: page
    property string host
    property int port
    property bool secure
    property bool sasl
    property string nickName
    property string realName
    property string userName
    property string password
    property string channel

    Settings {
        id: settings
        property alias host: page.host
        property alias port: page.port
        property alias secure: page.secure
        property alias sasl: page.sasl
        property alias nickName: page.nickName
        property alias realName: page.realName
        property alias userName: page.userName
        property alias password:page.password
        property alias channel: page.channel
    }

    signal accepted()
    signal rejected()
}
