import QtQuick 2.0
import QtQuick.Controls 1.4
import  ProRotoQml.Theme 1.0
import  ProRotoQml.Utils 1.0
import QtGraphicalEffects 1.0
Item {
    id: tFRoot
    height: 86
    width: parent.width / 1.07
    property color color: Colors.white
    property color borderColor: Colors.grey
    property int borderWidth: 0
    property int radius: 0


    property string icon: "qrc:/Artwork/help.svg"
    property string title
    property string textColor : Palette.text
    property int length: uname.text.length

    property string iconBackground: "transparent"
    property int iconBackgroundRadius: 0
    property int  iconBackgroundBorderWidth: 0
    property string  iconBackgroundBorderColor:  "transparent"

    property string text: uname.text

    property string placeholderText

    signal inputTextChanged()
    signal accepted()
    property int echoMode


    property bool usingPass: false
    property bool glowEnabledByMouse: false


    RectangularGlow{
        id:glow
        anchors.fill: parent
        anchors.margins: -5
        color:  "transparent"
        glowRadius: 5
        opacity: .77
        spread: 0.2
//        radius: 5

        MouseArea{
            anchors.fill: glow
            hoverEnabled: glowEnabledByMouse
            onEntered: glow.color = Colors.grey
            onExited:  glow.color = "transparent"
        }
    }

    Rectangle{
        anchors.fill: parent
        color: tFRoot.color
        radius: tFRoot.radius
        border{
            width: tFRoot.borderWidth
            color: tFRoot.borderColor
        }

        Rectangle{
            id: i
            height: parent.height - 4
            width: height
            color: iconBackground
            radius: iconBackgroundRadius
            border.width: iconBackgroundBorderWidth
            border.color: iconBackgroundBorderColor
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 4
            Image {
                source: tFRoot.icon
                height: parent.height - 5
                width: height
                anchors.centerIn: parent
            }
        }
        Text {
            id: mainText
            font.pixelSize: 22
            width: parent.width - i.width - 5
            wrapMode: Text.WrapAnywhere
            anchors.left: i.right
            anchors.leftMargin: 6
            anchors.top: parent.top
            anchors.topMargin: 12
            text: qsTr(tFRoot.title)
            color: textColor
        }


        TextField {
            id:uname
            anchors.horizontalCenter:  parent.horizontalCenter
            width: parent.width / 1.07
            text: tFRoot.text
            onFocusChanged: {
                focus === false ?  glow.color = "transparent" : glow.color =  Colors.grey
            }
//            focus: tFRoot.focus
            placeholderText: tFRoot.placeholderText
            echoMode: tFRoot.echoMode
            onAccepted: tFRoot.accepted()
            anchors.left: mainText.left
            anchors.top: mainText.bottom
            anchors.topMargin: 2
            onTextChanged: {
                if(tFRoot.usingPass === true){
                    tFRoot.inputTextChanged()
                    passStrength.update(text)
                    howStrong.value = passStrength.pwdStrength
                }
                else {
                    tFRoot.inputTextChanged()
                }
            }
        }
        Row{
            width : usingPass === true ?  parent.width / 1.07 : 0
            height: usingPass=== true  ? parent.height / 2.2 : 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: uname.bottom
            anchors.left: uname.left
            spacing: 8
            Label{
                id: passLab
                text:   passStrength.strenghtString
                font.pointSize: 10
                anchors.verticalCenter: howStrong.verticalCenter
                opacity: howStrong.opacity
            }

            ProgressBar{
                id:howStrong
                opacity: uname.length < 1 ? 0 : 1
                maximumValue: 100
                minimumValue: 0
                value: 0
                height: opacity > .2 ? parent.height / 2 : 0
                width: parent.width - (passLab.paintedWidth - 4)
            }
        }
    }
    PasswordStrength{id:passStrength}
}
