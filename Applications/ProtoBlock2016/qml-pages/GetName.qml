import QtQuick 2.6
import QtQuick.Layouts 1.3
import Material 1.0 as Material
import ProRotoQml.Theme 1.0
import Material.Extras 1.0
import QtQuick.Controls 2.0
import Material.ListItems 1.0 as ListItems

//import QtQuick.Controls.Material 2.0
//import QtQuick.Controls.Universal 2.0

Material.PopupBase {
    id: onboardsplash
    globalMouseAreaEnabled: false
    dismissOnTap: false

    property alias currentindex: stackView.currentIndex
    property int secret: 1
    property int start: 0


    opacity: showing ? 1 : 0
    visible: opacity > 0
//    overlayLayer: "dialogOverlayLayer"

    StackLayout  {
        id: stackView
        anchors.fill: parent
        currentIndex: 0//tabBar.currentIndex+1


        Material.View {
            anchors.centerIn: parent;
            anchors.fill: parent
            backgroundColor: themeroot.theme.primaryColor
            id: view

            Label {
                id: label1
                color: "white";
                text: qsTr("Welcome to Protoblock")
//                    Layout.columnSpan: 4
                verticalAlignment: Text.AlignVCenter
//                    font.pixelSize: 17
                font.bold: true
                textFormat: Text.RichText
                horizontalAlignment: Text.AlignHCenter
                enabled: true
                font.pixelSize:ProtoScreen.font( ProtoScreen.LARGE)
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true;
                anchors.topMargin: ProtoScreen.guToPx(10)
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }

            ColumnLayout {
//                anchors.centerIn: parent;
                anchors.topMargin: ProtoScreen.guToPx(20)
                id: columnLayout1
                clip: false
                spacing: ProtoScreen.guToPx(5)
                anchors.top: label1.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: ProtoScreen.guToPx(30)

                Material.Button {
                    text: "Create New Account"
                    Layout.preferredHeight: ProtoScreen.guToPx(8)
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true;
                    elevation: 1
                    onClicked: {
                        onboardsplash.close();
//                        rootLoader.source = "qrc:/Account.qml"
                        pageHelper.selectedTabIndex = themeroot.accountIndex;
                    }
                }

                Material.Button {
                    text: "Import Account"
                    Layout.preferredHeight: ProtoScreen.guToPx(8)
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true;
                    elevation: 1
//                    textColor: Material.Theme.light.textColor
                    onClicked: {
                        onboardsplash.close();
                        pageHelper.selectedTabIndex = themeroot.accountIndex;
                        rootLoader.source = "qrc:/Import-Export.qml"
                    }
                }

                Material.Button {
                    text: "Skip to App"
                    Layout.preferredHeight: ProtoScreen.guToPx(8)
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true;
                    elevation: 1
                    onClicked: {
                        onboardsplash.close();
                    }
                }
            }
        }

        Secret {
            isdisplay: currentindex === secret
        }

//            Account {}
//            Secret {}
//            Bitcoin {}
//            Balance {}
    }

    function show() {
        open()
    }
}
