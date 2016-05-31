import QtQuick 2.0
AbstractButton {
    id: categoryHeader
    //We use these to pass info to the headers of the dash layout
    property string icon
    property string label
    property bool folded: true
    property int count
    property int availableCount
    property bool uncollapsed
    property bool moving: false
    property var focusPath
    property var focusIndex
}
