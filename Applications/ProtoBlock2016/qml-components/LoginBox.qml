import QtQuick 2.0
import Material 1.0
import ProRotoQml.Theme 1.0
Item {
    Card{
        id: signInCard
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        elevation: 1
        Column{
            width: parent.width
            height: parent.height /2
            spacing: ProtoScreen.guToPx(3)
            anchors.centerIn: parent
            TextField{
                id: nameText
                width: parent.width / 1.07
                font.pixelSize:ProtoScreen.font(ProtoScreen.NORMAL)
                placeholderText: qsTr("Please enter in a name")
                anchors.horizontalCenter: parent.horizontalCenter
                onAccepted: clamNameButton.clicked();
            }
            Button{
                id: clamNameButton
                text: qsTr("Claim New Name")
                width: parent.width / 1.07
                elevation: 1
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    if (text === "Claim New Name" ){
                        // this is to SIGNUP
                        nameCheckBlank(nameText.text)
                    }else{
                        // THIS IS TO USE LAST YEARS PASS   WORD
                        var mypk = MiddleMan.importMnemonic(nameText.text)
                        if ( mypk === "" ){
                            importExportStatus = qsTr("Error: Import failed. Please try again")
                            myImportDialog.toggle()
                        }else{
                            importExportStatus = "Trying to import with key: " + mypk
                            nameText.text = ""
                            statusTxt.text = importExportStatus;
                            secretTxt.text = ""
                        }
                    }
                }
            }
            Button{
                text: qsTr("Skip")
                width: parent.width / 1.07
                elevation: 1
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    loginDialog.close();
                    rootLoader.source = "qrc:/Home.qml";
                }
            }
        }
    }
}
