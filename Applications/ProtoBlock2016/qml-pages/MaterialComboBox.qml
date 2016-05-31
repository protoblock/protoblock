import QtQuick 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    id: comboBox
    property bool expanded : false
    property alias expandedHeight: listView.height
    property variant model
    property alias currentIndex: listView.currentIndex
    property string currentText
    property alias elevation: vCard.elevation
    property alias spacing: listView.spacing
    signal clicked()
    ListItems.Standard{
        id: vCard
        width: parent.width
        elevation: 1
        height: 48
        text: currentText
        onClicked: {
            if(comboBox.expanded === true ){
                comboBox.expanded = false
                anamateClose.start()
            }else if (comboBox.expanded === false){
                comboBox.expanded = true
                anamateOpen.start()
            }
        }
    }
    NumberAnimation{
        id: anamateOpen ;
        target:  viewCard;
        duration: 600;
        easing.type: Easing.OutQuad
    }
    NumberAnimation{
        id: anamateClose ;
        target:  viewCard;
        duration: 600;
        easing.type: Easing.InQuad
    }
    Card{
        id: viewCard
        width: parent.width
        height: listView.height
        anchors.top: vCard.bottom
        anchors.topMargin: 1
        visible: comboBox.expanded === true ? true : false
        ListView{
            id: listView
            width: parent.width
            height: comboBox.expanded === true ?  ((48 + spacing) * count )   :  0
            clip: true
            interactive: true
//            onCountChanged: console.log(count)
            visible: comboBox.expanded === true ? true : false
            model: comboBox.model
            delegate: ListItems.Standard{
                height: 48
//                focus: true
                text: modelData
                onClicked: {

                    console.log(text)
                    comboBox.currentText = text
                    comboBox.currentIndex = index
                    comboBox.expanded = false
                    anamateClose.start()

                }
            }
        }
        Scrollbar{
            flickableItem: listView
        }
    }
}
