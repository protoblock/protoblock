import QtQuick 2.0
import QtQuick.Controls 1.4 as Contols
import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

Item {
    id: loginPage
    property string  defaultTxt: "A Protoblock Player name is your fantasy identity, and will be displayed on the projections leaderboard. " +
                                 "\nClaim your name! Chose your Twitter handle or fantasy team name";

    Card{
        id: loginCard
        width: parent.width
        height:  parent.height
        elevation: 5
        Rectangle{
            id: aboutHeader
            width: parent.width
            height:   parent.height / 10
            color: "#2196f3"
            Label  {
                text: "Please pick a username"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                font.pixelSize:ProtoScreen.font(ProtoScreen.LARGE)
                font.bold: true
                color: "white"
            }
        }
        Image {
            id: logo
            source: "qrc:/logoFinal.png"
            width: parent.width / 1.07
            height: parent.height / 5.7
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: aboutHeader.bottom
            anchors.topMargin: parent.height / ProtoScreen.guToPx(2.75)
        }
        Label  {
            id: welcomeTxt
            text: defaultTxt
            anchors.top: logo.bottom
            anchors.topMargin: ProtoScreen.guToPx(2)
            width: loginCard.width / 1.07
            height: paintedHeight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pixelSize:ProtoScreen.font(ProtoScreen.NORMAL)
        }
        Card{
            id: signInCard
            width: parent.width / 1.07
            height: parent.height / 1.5 - (aboutHeader.height + logo.height ) + welcomeTxt.paintedHeight
            anchors.top: welcomeTxt.bottom
            anchors.topMargin: ProtoScreen.guToPx(1)
            anchors.horizontalCenter: parent.horizontalCenter
            Column{
                width: parent.width
                height: parent.height / 2
                anchors.centerIn: parent
                spacing: ProtoScreen.guToPx(1)
                TextField{
                    id: nameText
                    width: parent.width / 1.07
                    height: ProtoScreen.guToPx(8)
                    font.pixelSize:ProtoScreen.font(ProtoScreen.NORMAL)
                    placeholderText: "Please enter in a name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onAccepted: clamNameButton.clicked();
                }
                Button{
                    id: clamNameButton
                    text: "Claim New Name"
                    width: parent.width / 1.07
                    height: ProtoScreen.guToPx(8)
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        if (text === "Claim New Name" ){
                            // this is to SIGNUP
                            nameCheckBlank(nameText.text)
                        }else{
                            // THIS IS TO USE LAST YEARS PASS   WORD
                            var mypk = MiddleMan.importMnemonic(nameText.text)
                            if ( mypk === "" ){
                                importExportStatus = "Error: Import failed. Please try again"
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
                    text: "Skip"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: ProtoScreen.guToPx(8)
                    onClicked: {
                        loginDialog.close();
                    }
                }


//                Button{
//                    text: "Already Have a Name"
//                    width: parent.width / 1.07
//                    elevation: 5
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    height: ProtoScreen.guToPx(8)
//                    onClicked: {
//                        nameText.text = ""
//                        if (text === "Already Have a Name" ){
//                            clamNameButton.text = "Submit"
//                            welcomeTxtAnimation.start()
//                            nameText.placeholderText = "Please enter in your last years password"
//                            welcomeTxt.text = "Last year when you played you recived a 12 word password. You can now enter that into the the box below to re-cliam your fantasy name for 2016. If you do not remeber the 12 word secert you can open up trading football 2015 and click on export my mnemonic and this will give you your password. For further help contact the protoblock team via email <contact@protoblock.com>. "
//                            text = "Back"
//                        }
//                        else
//                        {
//                            nameText.placeholderText = "Please enter in a new fantasy name"
//                            welcomeTxtAnimation.start()
//                            welcomeTxt.text = defaultTxt
//                            text = "Already Have a Name"
//                            clamNameButton.text = "Claim New Name"
//                        }
//                    }
//                }
            }
        }

        Label{
            id: errorLabel
            color: Theme.accentColor
            width: parent.width
            horizontalAlignment:  Text.AlignHCenter
            font.family: "Roboto"
            font.pixelSize: ProtoScreen.guToPx(2.75)
            wrapMode: Text.WordWrap
            anchors.top: signInCard.bottom
            anchors.topMargin: paintedHeight*2
            onTextChanged: scale = scale +1
            Behavior on scale {
                NumberAnimation{
                    duration: 1200 ; from: 0 ; to: 1 ; easing.type: Easing.OutBounce
                }
            }
        }
    }//card

    function nameCheckBlank(s) {
        if ( s.length === 0 ) {
            return
        }
        else if ( s.length > 45) {
            errorString = "Sorry but that fantasy name is just to long. Please try again."
            accountErrorDialog.toggle()
        }
        else {
            MiddleMan.checkname(s)
        }
    }

    SequentialAnimation{
        id: welcomeTxtAnimation
        NumberAnimation { target: welcomeTxt ; property: "opacity"; from: 1; to: 0; duration: 1; }
        NumberAnimation { target: welcomeTxt ; property: "opacity"; from: 0; to: 1; duration: 600; }
    }
}
