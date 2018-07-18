import QtQuick 2.4
import ProRotoQml.Theme 1.0
import Material 1.0

Card{
    id: cardWithBanner
    elevation: 1
    // pass up the banner so we can change global
    property string  bannerColor: themeroot.theme.primaryColor
    property string bannerText
    // this is the main children like flickable objects
    property alias  mainobject: mainPage.children
    property alias  bannerObject: bannerContent.children
    // this is the banner  children like a Row of objects
    property int margin: ProtoScreen.guToPx(1)
    // helper section of this banner
    property string helperHeader
    property string helperTxt
    property bool helpShown: false
    property string anchrosType: "default"
    property bool bannerbottom: false
    Banner{
        id: cBan
        width: parent.width
        text: cardWithBanner.bannerText
        anchors.top: !bannerbottom ? parent.top : theitem.bottom
//        anchors.topMargin: !bannerbottom ? 0 : theitem.height
        backgroundColor: cardWithBanner.bannerColor
        helperHeader: cardWithBanner.helperHeader
        helperTxt: cardWithBanner.helperTxt
        helpShown: cardWithBanner.helpShown
        anchrosType: cardWithBanner.anchrosType
        Item{
            id: bannerContent
            anchors.fill: cBan
        }
    }
    Item{
        id: theitem
        width: parent.width
        height: mainPage.height
        anchors.top: bannerbottom ? parent.top : cBan.bottom
//        anchors.topMargin: bannerbottom ? 0 : cBan.height
//        anchors.bottom: bannerbottom ? cBan.top : parent.bottom
        Rectangle{
            id: mainPage
            anchors.fill: parent
            color:"transparent"
        }
    }
}
