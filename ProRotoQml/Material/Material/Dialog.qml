import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Material 1.0
import ProRotoQml.Theme 1.0
import Material.Extras 1.0

PopupBase {
    id: dialog
    overlayLayer: "dialogOverlayLayer"
    overlayColor: Qt.rgba(0, 0, 0, 0.3)
    opacity: showing ? 1 : 0
    visible: opacity > 0
    width:{
        if (ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "tablet" || ProtoScreen.formFactor === "phablet" ) {
            Screen.width / 1.07
        }else{
            parent.width / 1.8
        }
    }
    height:{
        if (ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "tablet"|| ProtoScreen.formFactor === "phablet"){
            Screen.height / 1.2
        }else{
            parent.height / 1.5
        }
    }
    Component.onCompleted: console.log("HERE IS THE AVAIL HEIGHT " + ProtoScreen.availableHeight )
    property int contentMargins: ProtoScreen.guToPx(3)
    property int minimumWidth: ProtoScreen.guToPx(37.5)
    property int minimumHeight: ProtoScreen.guToPx(37.5)
    property alias title: titleLabel.text
    property alias text: textLabel.text
    property alias negativeButton: negativeButton
    property alias positiveButton: positiveButton
    property string negativeButtonText: "Cancel"
    property string positiveButtonText: "Ok"
    property alias positiveButtonEnabled: positiveButton.enabled
    property bool hasActions: true
    property bool floatingActions: false
    default property alias dialogContent: column.data

    signal accepted()
    signal rejected()

    anchors {
        centerIn: parent
        verticalCenterOffset: showing ? 0 : -(dialog.height/3)
        Behavior on verticalCenterOffset { NumberAnimation { duration: 200 } }
    }

    Behavior on opacity { NumberAnimation { duration: 200 } }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            closeKeyPressed(event)
        }
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            closeKeyPressed(event)
        }
    }

    function closeKeyPressed(event) {
        if (dialog.showing) {
            if (dialog.dismissOnTap) {
                dialog.close()
            }
            event.accepted = true
        }
    }

    function show() {
        open()
    }

    View {
        id: dialogContainer
        anchors.fill: parent
        elevation:   5
        radius: ProtoScreen.guToPx(.5)
        backgroundColor: "white"
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: false
            onClicked: {
                mouse.accepted = false
            }
        }

        Rectangle {
            anchors.fill: content
        }

        Flickable {
            id: content
            contentWidth: column.implicitWidth
            contentHeight: column.height + (column.height > 0 ? contentMargins : 0)
            clip: true
            anchors {
                left: parent.left
                right: parent.right
                top: headerView.bottom
                bottom: floatingActions ? parent.bottom : buttonContainer.top
            }

            interactive: contentHeight > height

            onContentXChanged: {
                if(contentX != 0 && contentWidth <= width)
                    contentX = 0
            }

            onContentYChanged: {
                if(contentY != 0 && contentHeight <= height)
                    contentY = 0
            }

            Column {
                id: column
                width: content.width - 2 * contentMargins
                spacing: ProtoScreen.guToPx(1)
                anchors {
                    left: parent.left
                    leftMargin: contentMargins
                }
            }
        }

        Scrollbar {
            flickableItem: content
        }

        Item {
            height: headerView.height
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            View {
                backgroundColor: "white"
                elevation: content.atYBeginning ? 0 : 1
                fullWidth: true
                radius: dialogContainer.radius
                height: parent.height
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
            }
        }


        Column {
            id: headerView
            spacing: 0
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                leftMargin: contentMargins
                rightMargin: contentMargins
            }

            Item {
                width: parent.width
                height: contentMargins
                visible: titleLabel.visible || textLabel.visible
            }

            Label {
                id: titleLabel
                width: parent.width
                wrapMode: Text.Wrap
                style: "title"
                visible: title != ""
            }

            Item {
                width: parent.width
                height: ProtoScreen.guToPx(2.5)
                visible: titleLabel.visible
            }

            Label {
                id: textLabel
                width: parent.width
                wrapMode: Text.Wrap
                style: "dialog"
                color: Theme.light.subTextColor
                visible: text != ""
            }

            Item {
                width: parent.width
                height: contentMargins
                visible: textLabel.visible
            }
        }

        Item {
            id: buttonContainer
            anchors {
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            //  well then that was stupid
            height: hasActions ? ProtoScreen.guToPx(6.5) :  0

            View {
                id: buttonView
                height: parent.height
                backgroundColor: floatingActions ? "transparent" : "white"
                elevation: content.atYEnd ? 0 : 1
                fullWidth: true
                radius: dialogContainer.radius
                elevationInverted: true
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left
                }

                Button {
                    id: negativeButton
                    visible: hasActions
                    text: negativeButtonText
                    textColor: Theme.accentColor
                    context: "dialog"
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: positiveButton.visible ? positiveButton.left : parent.right
                        rightMargin: ProtoScreen.guToPx(1)
                    }

                    onClicked: {
                        close();
                        rejected();
                    }
                }

                Button {
                    id: positiveButton
                    visible: hasActions
                    text: positiveButtonText
                    textColor: Theme.accentColor
                    context: "dialog"

                    anchors {
                        verticalCenter: parent.verticalCenter
                        rightMargin: ProtoScreen.guToPx(1)
                        right: parent.right
                    }
                    onClicked: {
                        close()
                        accepted();
                    }
                }
            }
        }
    }
}
