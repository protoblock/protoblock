import QtQuick 2.4

Image {
    property bool folded: true
    rotation: folded ? 0 : 90
    source: "qrc:/report_problem.png"
    width: sourceSize.width
    height: sourceSize.height
    smooth: true
    fillMode: Image.PreserveAspectFit
    Behavior on rotation {NumberAnimation {duration: 100}}
}
