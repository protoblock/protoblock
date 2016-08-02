import QtQuick 2.0

Item {
    id: mTxFd
    property string text
    property string color: "white"
    property alias helperText
    property string placeHolderText
    property bool bold: false
    property string passwordCharacter : "*"
    property bool  readOnly: false

    signal accepted()
    signal clear()
    signal copy()
    signal cut()
    signal deselect()
    signal insert(int position, string text)

    property alias  capitalization: input.font.capitalization
    property alias family : input.font.family
    property alias italic : input.font.italic
    property alias letterSpacing :  input.font.letterSpacing
    property alias pixelSize :  input.font.pixelSize
    property alias pointSize :  input.font.pointSize
    property alias strikeout :  input.font.strikeout
    property alias underline : input.font.underline
    property alias weight :   input.font.weight
    property alias wordSpacing :  input.font.wordSpacing
    property alias length: input.length
    property alias echoMode: input.echoMode
    property alias effectiveHorizontalAlignment: input.effectiveHorizontalAlignment
    property alias verticalAlignment : input.verticalAlignment
    property alias cursorVisible: false
    property alias contentHeight: input.contentHeight
    property alias contentWidth: input.contentWidth
    property alias wrapMode: input.wrapMode
    property alias validator: input.validator



    TextInput{
        id: input
        width: parent.width
        text: mTxFd.text
        onAccepted: mTxFd.accepted()
    }

    // bottom bar
    Rectangle{
        id: botBar
    }

}
