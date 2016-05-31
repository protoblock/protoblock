import QtQuick 2.0
import "Utils.js" as Utils
import "QChart.js" as Charts
import Material 1.0
Item {

    Chart{
        id: chart_line;
        width: parent.width / 1.07
        height: parent.height / 1.3
        chartAnimated: true;
        chartAnimationEasing: Easing.InOutElastic;
        chartAnimationDuration: 1200;
        chartData: Utils.ChartLineData
        chartType: Charts.ChartType.LINE;
    }

    Row{
        width: parent.width
        height: parent.height - chart_line.height
        anchors.bottom: parent.bottom
        Button{
            text: "Repaint"
            onClicked: chart_line.repaint()
        }
        Button{
            text: "Request paint"
            onClicked: chart_line.requestPaint()
        }
    }
}
