import QtQuick 2.0
import Material 1.0

Item {
    id: boxTextRoot
    property int elevation : 1
    property string defaultText: "0"
    property alias value:  input.text
    property bool showNumbers: false
    Card{
        width: parent.width
        height: parent.height
        elevation: boxTextRoot.elevation
        TextField{
            id: input
            width: parent.width
            height: parent.height
            text: defaultText
            maximumLength: 1
            font.pixelSize: parent.height / 1.07
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            echoMode: {
                if(showNumbers === false){
                TextInput.Password
                }else{
                    TextInput.Normal
                }
            }
            validator: IntValidator{bottom: 0; top: 9;}

        }
    }
}
