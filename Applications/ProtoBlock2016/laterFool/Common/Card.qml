import QtQuick 2.4
import  ProRotoQml.Theme 1.0
View {
    width: Unit.dp(300)
    height: Unit.dp(250)
    elevation: flat ? 0 : 1

    property bool flat: false

    border.color: flat ? Qt.rgba(0,0,0,0.2) : "transparent"
    radius: fullWidth || fullHeight ? 0 : Unit.dp(2)

}



