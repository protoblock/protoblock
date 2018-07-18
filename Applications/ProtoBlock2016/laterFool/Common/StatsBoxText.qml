import QtQuick 2.0

Rectangle{
    id: statsBox
    property string headerText
    property string mainText
    width:  20
    height: 20
    color: "transparent"
    Column{
        anchors.fill: parent
        BoxText{
            id: headerTxt
            height:     parent.height / 3
            width: parent.width
            text: headerText
        }
        BoxText{
            height: parent.height - headerTxt.height
            width: parent.width
            text: mainText
        }
    }
}
