import QtQuick 2.0
import QmlSsh 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import Material 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Files 1.0


Item {
    property string cryptFile:MiddleMan.encyptPath +uname + "-lock"
    Card{
        height:parent.height / 1.07
        width: parent.width / 1.07
        elevation: 5
        anchors.centerIn: parent
        Column{
            width: parent.width
            height: parent.height
            spacing: 22

            Rectangle{
                id: header
                width: parent.width
                height: parent.height / 12
                color: Colors.blue
                Text {
                    id: t
                    height: parent.height
                    width: parent.width
                    anchors.bottom: header.bottom
//                    horizontalAlignment: Image.AlignHCenter
                    text: qsTr("Please Pick a four diget Login pin " )
                    font.pointSize: 32
                    wrapMode: Text.WordWrap
                    color: "white"
                }
            }

            Tumbler {
                width: parent.width / 1.07
                height: parent.height / 4
                anchors.horizontalCenter: parent.horizontalCenter
                  TumblerColumn {
                      model: 9
                  }
                  TumblerColumn {
                      model: 9
                  }
                  TumblerColumn {
                      model: 9
                  }
                  TumblerColumn {
                      model: 9
                  }
              }
            Row{
                width: parent.width / 1.07
                height: parent.height / 6
                anchors.horizontalCenter: parent.horizontalCenter
                BoxText{
                    id: spinOne
                    width: parent.width / 8
                    height: parent.height
                }
                BoxText{
                    id: spinTwo
                    width: parent.width / 8
                    height: parent.height
                }
                BoxText{
                    id: spinThree
                    width: parent.width / 8
                    height: parent.height
                }
                BoxText{
                    id: spinFour
                    width: parent.width / 8
                    height: parent.height
                }

                CheckBox{
                    text: "Show Password"
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged: {
                        if(checked === true){
                            unCheckPin(true)
                        }else {
                            unCheckPin(false)
                        }
                    }
                }
            }

            Button{
                elevation: 1
                text: "ecypt file "
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {

                    fileWriter.fileName = cryptFile
                    fileWriter.incoming = MiddleMan.encyptPath + "secert3.out"
                    fileWriter.exec()

                    secureNodes.fileName = cryptFile
                    secureNodes.password = getPin()
                    secureNodes.encryptFile(cryptFile)
                }
            }



            Button{
                elevation: 1
                text: "decypt file "
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    secureNodes.password = getPin()
                    if ( secureNodes.decryptFile(
                                cryptFile ,
                                cryptFile.replace("-lock" ,"-unlock")
                                )
                            )
                    {
                        console.log("File Decypted")
                        // remove the unlock file.  User has now loged into the System
                        fileWriter.remove(cryptFile.replace("-lock" ,"-unlock"))
                    }
                    else{
                        console.log("Error in Dencryption")
                    }
                }
            }
        }
    }

    QmlFile{
        id: fileWriter
        type: QmlFile.Write
    }
    SecureNodes{
        id:secureNodes
    }

    function unCheckPin(b){
        spinOne.showNumbers = b
        spinTwo.showNumbers = b
        spinThree.showNumbers = b
        spinFour.showNumbers = b
    }

    function getPin(){
        var str = ""
        str = spinOne.value + spinTwo.value +spinThree.value + spinFour.value
        return str
    }
}
