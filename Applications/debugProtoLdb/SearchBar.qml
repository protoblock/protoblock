import QtQuick 2.0
import QtQuick.Controls 1.4
import ProRotoQml.Models 1.0
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0
import "Common"
Item {
    id: sortSearch
    property alias currentText: searchInput.text
    property variant model
    property string selectionType
    width:  120
    height: 32

    Rectangle{
        anchors.fill: parent
        color: "green"
        TextField{
            id: searchInput
            width: parent.width
            height: parent.height
            onActiveFocusChanged: {
                if (focus === true){
                    text = ""
                }else if (focus === false){
//                    console.log("MOUSE ?        " +focus)
                }
            }

            onActiveFocusOnPressChanged:
                console.log("MOUSE    ")
            }

        Rectangle{
            id: dropdown
            height:searchInput.focus === true ?  searchInput.height * searchView.count : 0
            width: searchInput.focus === true ?  searchInput.width  : 0
            anchors.top: searchInput.bottom
            radius: 5
            border{width:searchInput.focus === true ? 3 : 0 ; color: "#77000000"}
            Behavior on height{NumberAnimation{duration: 700 ; easing.type: Easing.InOutQuad}}
            ListView{
                id: searchView
                spacing:  5
                height: dropdown.height
                width: dropdown.width
                model: SortModel {
                    source:searchModel
                    sortCaseSensitivity: Qt.CaseInsensitive
                    filterString: "*" + searchInput.text + "*"
                    filterSyntax: SortModel.Wildcard
                    filterCaseSensitivity: Qt.CaseInsensitive
                }
                delegate: Card{
                    id: dropDownDel
                    width: sortSearch.width / 1.07
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 5
                    visible: searchInput.focus === true ? true : false
                    elevation: searchInput.focus === true ? 10 : 0
                    height: searchInput.focus === true ? searchInput.height : 0
                    Text {
                        id: dropdownText
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        visible: searchInput.focus === true ? true: false
                        font.pixelSize:  parent.height / 1.8
                        wrapMode: Text.WordWrap
                        text: messageType
                        anchors{
                            left: parent.left
                            leftMargin: 5
                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered:{
                             dropdownText.color = Colors.white
                            dropDownDel.color = Colors.blueGrey
                        }
                            onExited:{
                                dropdownText.color = Colors.black
                                dropDownDel.color = Colors.white
                        }
                                onClicked:{

                            searchInput.text = messageType
                            searchInput.focus = false
                        }
                    }
                }
            }
        }


        ListModel{id:searchModel}
        Component.onCompleted: {
            var li = [LdbHashReaderTool.types]
            var lis = li.toString();
            var list = lis.split(",")
            console.log(list.length)
            for (var i = 0 ; i < list.length ; i++){
                searchModel.append({"messageType" : list[i] })
            }
        }
    }
}
