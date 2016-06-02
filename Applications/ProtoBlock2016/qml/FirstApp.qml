import QtQuick 2.0
import Material 1.0 as Material
import ProRotoQml.Protoblock 1.0
import QtQuick.Window 2.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0
 Window{
    id: root
    width: Screen.width
    height: Screen.height
    visible: true
    property string  uname: "NULL"
    property string err
    property int loginCardScale: 1
    PageHelper{
        id: pageHelper
        width: parent.height
        height: parent.height
        title: "Please Claim a name"
        buttonsEnabled: false
        object :
            Loader{
            id: rootLoader
            anchors.fill: parent
            sourceComponent: login
        }

        Component{
            id: login
            GetName{
                anchors.fill: parent
            }
        }


        Component{
            id: afterLogin
            Material.Card{
                height: parent.height / 1.07
                width: parent.width / 1.07
                elevation: 5
                Text {
                    width: parent.width
                    height: parent.height
                    verticalAlignment: Image.AlignVCenter
                    horizontalAlignment: Image.AlignHCenter
                    text: qsTr("Thanks for claiming a name more to come soon")
                    font.pointSize: 64
                    wrapMode: Text.WordWrap
                }
            }
        }


        Connections {
            target: MiddleMan
            onNameCheckGet: {
                //            console.log("onNameCheckGet " + status  + " \n" +  name )
                if(status === "true" )
                {
                    console.log("name is not taken")
                    MiddleMan.signPlayer(uname)
                }
                else
                {
                    err = "This name is taken if you feel that you are this person. You can go back and claim you last years name.  Of if you need help feel free to send a email to support@protoblock.com"
                    //                    root.loginCardScale = 0
                    //                    loginErrorDialog.open()
                    //                    root.errorString =  err
                }
            }

            onNameStatusChanged: {
                uname = MiddleMan.playersName;
                pageHelper.title = "Thanks For claiming a name"
                rootLoader.sourceComponent = afterLogin

            }

            onUsingFantasyName: {
                uname = MiddleMan.playersName;
                rootLoader.sourceComponent = afterLogin
                pageHelper.title = "Thanks For claiming a name"
            }
        }

    }
}
