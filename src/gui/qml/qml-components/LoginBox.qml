import QtQuick 2.12
import Material 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Utils 1.0
Item {
    width: parent.width
    height: signInCard.height + nameText.height +  (clamNameButton.height * 2) + ProtoScreen.guToPx(3)
    Card{
        id: signInCard
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        elevation: 8
        Column{
            width: parent.width
            height: parent.height /2
            spacing: ProtoScreen.guToPx(4) //Fix Me - android hack was 3
            anchors.centerIn: parent
            TextField{
                id: nameText
                width: parent.width / 1.07
                height: ProtoScreen.guToPx(6)
                font.pixelSize:ProtoScreen.font(ProtoScreen.NORMAL)
                helperText: qsTr("Please enter in a name")
                anchors.horizontalCenter: parent.horizontalCenter
                onAccepted: clamNameButton.clicked();
                inputMethodHints: Qt.ImhNoPredictiveText;
            }
            Button{
                id: clamNameButton
                text: qsTr("Claim New Name")
                width: parent.width / 1.07
                elevation: 5
                backgroundColor: Colors.blue
                height: ProtoScreen.guToPx(5)
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
//                    if (text === "Claim New Name" ){
                        // this is to SIGNUP
                        nameCheckBlank(nameText.text)
//                    }else{
//                        // THIS IS TO USE LAST YEARS PASS   WORD
//                        var mypk = MiddleMan.importMnemonic(nameText.text)
//                        if ( mypk === "" ){
//                            importExportStatus = qsTr("Error: Import failed. Please try again")
//                            myImportDialog.toggle()
//                        }else{
//                            importExportStatus = "Trying to import with key: " + mypk
//                            nameText.text = ""
//                            statusTxt.text = importExportStatus;
//                            secretTxt.text = ""
//                        }
//                    }
                }
            }
            Button{
                text: "Skip"
                backgroundColor: Colors.blue

                width: parent.width / 1.07
                height: ProtoScreen.guToPx(5)
                elevation: 5
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    rootLoader.source = "qrc:/Projections.qml";
                    loginDialog.close();
                }
            }
        }
    }
}
