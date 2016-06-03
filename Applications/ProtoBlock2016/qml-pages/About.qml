import QtQuick 2.0
import Material 1.0
Item {
    Component.onCompleted:  pageHelper.title = "About Protoblock"
    Card{
        width: parent.width / 1.07
        height:  parent.height / 1.07
        anchors.centerIn: parent
        elevation: 5
        Rectangle{
            id: aboutHeader
            width: parent.width
            height:   parent.height / 10
            color: "#2196f3"
            Label  {
                text: "About Protoblock"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                font.family: "Roboto"
                font.pixelSize: parent.height / 2
                font.bold: true
                color: "white"
            }
        }


        Flickable{
            contentWidth:parent.width / 1.07
            contentHeight:  parent.height * 2
            interactive: true
            boundsBehavior: Flickable.StopAtBounds
            width:parent.width
            height: parent.height  - (aboutHeader.height + 10 )
            anchors.top: aboutHeader.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true
            Column{
                id: aboutCol
                width: parent.width
                height: parent.height
                //            anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Image {
                    source: "qrc:/logoFinal.png"
                    width: parent.width / 1.07
                    height: parent.height / 12
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label{
                    font.family: "Roboto"
                    font.pixelSize: Qt.platform.os === "android" ? 48 : 32
                    font.bold: true
                    text: "  New Era"
                }
                ThinDivider{}
                Label{
                    font.family: "Roboto"
                    font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                    width: parent.width /  1.07
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Protoblock is the first blockchain gaming company for fantasy football. A new era of fantasy football began with the launch of the first ever blockchain based fantasy football game for 2015. The upcoming 2016 season will take this one step farther with our new mobile apps, and much, much more."
                }

                Label{
                    font.family: "Roboto"
                    font.pixelSize: Qt.platform.os === "android" ? 48 : 32
                    font.bold: true
                    text: "  Our Mission"
                }
                ThinDivider{}

                Label{
                    font.family: "Roboto"
                    font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                    width: parent.width /  1.07
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Protoblock is on a mission to create a new fantasy economy, were traditional fantasy football skill is valued above all. We combine our passion for fantasy football with our software development expertise.\n
At Protoblock, we believe fantasy football is a skill, and our experts deserve to be recognized and rewarded like sports or movie stars."
                }
                //Fantasy Football players mimic the role of a real NFL general managers.
                //50 million people play fantasy.
                //Fantasy generates more revenue than the entire NFL."
            }
        }
    }
}
