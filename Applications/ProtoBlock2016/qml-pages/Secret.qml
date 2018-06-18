import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0
import ProRotoQml.Theme 1.0


Pane {
    id: secretpage

    Component.onCompleted: {
        console.log("secret " + MiddleMan.getSecret())
    }

    TextEdit {
        id: cliphelper
        visible: false
    }

//    TextEdit {
//        id: pasthelper
//        visible: false
//    }



    property var secretWordsIn: {
//        return !isdisplay ? pastWordsin :
        return !isdisplay ? pastWordsin : MiddleMan.getSecret().split(" ");
    }

    property var pastWordsin: []
//    property bool isimport: window.doimport && !CoinSale.secretIsVerified
//    property bool isverify: window.doverify && !CoinSale.secretIsVerified
    property bool isdisplay: false//!isimport && !isverify
    property var secretOut: []

    property bool didcopy: false
//    property bool didpaste: false

    readonly property int itemWidth:  (secretpage.availableWidth / 6) * 5

    Column {
        anchors.fill: parent
        anchors.topMargin: ProtoScreen.guToPx(2)
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            spacing: 4
            anchors.horizontalCenter: parent.horizontalCenter
            width: (secretpage.width / 6) *5

            Label {
                wrapMode: Label.Wrap
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                width: (secretpage.width / 6) *5

                text: "Write down your 12 word recover secret!"

//                       isdisplay ?
//                          ( CoinSale.secretIsVerified ?
//                               "Write down your 12 word recover secret!"
//                             :
//                               "Step 2: Write down your 12 word recover secret!"  )
//                        :  (  isverify ?
//                                "Step 2: Verify secret words."
//                            :
//                                "Step 1: Import secret words.");
            }

            Label {
                wrapMode: Label.Wrap
                anchors.horizontalCenter: parent.horizontalCenter
                width: (secretpage.width / 6) *5

                text: "These words are needed to obtain your Fantsybits. Hint: Click to copy secret to clipboard"

//                    isdisplay ?
//                          ( CoinSale.secretIsVerified ?
//                               "These words are needed to obtain your Fantsybits and to recover stuck bitcoins. Hint: Click to copy secret to clipboard"
//                             :
//                               "These words are needed to obtain your Fantsybits and to recover stuck bitcoins. Hint: Click to copy secret to clipboard" )
//                        :  (  isverify ?
//                                "Type in or paste (right click) your 12 secret words, your bain wallet. Verify your brain wallet before sending bitcoin to funding address!"
//                            :
//                                "Type in or paste (right click) your 12 secret words, your bain wallet. Verify your brain wallet before sending bitcoin to funding address! ");

                font.pixelSize: 12
            }
        }


        Frame {
            anchors.horizontalCenter: parent.horizontalCenter
            id: rec1

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                width: itemWidth

                GridView {
                    width: parent.width
                    implicitHeight: 200
                    id: grid
                    cellHeight: 200 / 6
                    cellWidth: itemWidth / 2

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: 12
                    flow: GridView.FlowTopToBottom
                    delegate: secretDelegate
                    MouseArea {
                        id: ma
                        anchors.fill: parent
                        onClicked: {
                            if ( isdisplay ) {
                                console.log("copying ")
                                cliphelper.text = MiddleMan.getSecret()
                                console.log("copying 2")

                                cliphelper.selectAll()
                                cliphelper.cut()
                                console.log("copying3 ")

                                console.log("copying4 ")
                                rec1.ToolTip.show("Secret Copied to Clipboard" ,5000)
                                didcopy = true
                            }
                            else {
                                pasthelper.text = ""
                                pasthelper.paste()
                                console.log("pste " + pasthelper.text)
                                pastWordsin = pasthelper.text.trim().split(" ")
                                console.log("PASTING3 ")
                                rec1.ToolTip.show("Secret Pasted from Clipboard" ,5000)
                                console.log("PASTING4 ")
                            }
                        }

                        acceptedButtons: Qt.LeftButton//!isdisplay ? Qt.RightButton : Qt.LeftButton
                    }

                }
            }
        }

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10;
            Button {
                id: button1
                enabled: true//!CoinSale.secretIsVerified //CoinSale.currName === "" ||  isverify
                text: "OK got it!";//isimport ? "Import" : ( isverify ? "Verify Secret" : "OK got it!")
                onClicked: {
//                    if ( !isdisplay ) {
//                        var words = secretOut.join(" ")  ;
//                        console.log("clicked: " + words)
//                        if ( words !== "" ) {
//                            if ( isimport) {
//                                console.log("try import")

//                                rec1.ToolTip.show("Try Import" ,500)
//                                CoinSale.doimport(words);
//                            }
//                            else {
//                                console.log("try verify")
//                                console.log("try verify 2")
//                                CoinSale.verify(words);
//                                console.log("try verify 3")
//                            }
//                        }
//                        else {
////                            rec1.ToolTip.show("bad import" ,2000)
////                            CoinSale.toolPop("bad import");
//                            rec1.ToolTip.show("Bad Import" ,2000)

//                            CoinSale.set_currStatus("bad import")
//                        }
//                    }
//                    else {
                        if ( didcopy ) {
                            cliphelper.text = "secret cleared from clipboard"
                            cliphelper.selectAll()
                            cliphelper.copy()
                            cliphelper.text = ""
                        }

                        onboardsplash.close();

//                        CoinSale.secretOk();
//                    }
                }
                anchors.horizontalCenter: parent.horizontalCenter
                width: itemWidth / 2.5
            }

//            Text {
//                    id: forgotbut
//                    enabled: isverify
//                    visible: isverify

//                    textFormat: Text.RichText
//                    text: "Forgot secret?  <a href=\"import\">Click to recover</a>"
//                    font.pixelSize: 10
//                    onLinkActivated: {
//                        CoinSale.forgot()
//                    }
//            }
        }
    }


    Component {
        id: secretDelegate

        Item {
            width: grid.cellWidth
            height: grid.cellHeight

            RowLayout {
                height: parent.height
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Label {
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: index < 6 ? Text.AlignLeft : Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -2
                    Layout.preferredWidth: parent.width * .20
                    id: lb
                    text: (index < 9 ? " " : "") +(index+1) + ": "
                    font.pixelSize: 16

                }
                TextField {
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignBottom
                    Layout.alignment: Qt.AlignLeft
                    font.pixelSize: 16
                    text: (secretWordsIn.length <= index) ? "" :  secretWordsIn[index]
                    font.bold: true
                    readOnly: true//secretpage.isdisplay
                    onTextChanged: {
//                        console.log(index + "text changed " + text)
                        secretOut[index] = text
                    }
                }
            }
        }
    }

}
