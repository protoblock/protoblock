import QtQuick 2.12
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Particles 2.0
Window {
    id: splash
    color: "transparent"
    title: "Splash Window"
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    property int timeoutInterval: 6000
    signal timeout
    width: Screen.width
    height: Screen.height
    ParticleSystem {id: sys}
    ImageParticle {
        id: up
        system: sys
        source:  "qrc:/logoOnly.png"
        autoRotation: true
        rotation: -90
    }
    Emitter {
        anchors.centerIn: parent
        system: sys
        emitRate: 100
        size: 40
        sizeVariation: .5
        lifeSpan: 10000
        velocityFromMovement: 1.0
        velocity: AngleDirection {
            angle: 360
            angleVariation: 360
            magnitude: 300
            magnitudeVariation: 100
        }
    }
    BusyIndicator{
        width: (Screen.width / 2) + 20
        height: (Screen.width / 2) + 20
        running: splash.visible ? true : false
        anchors.centerIn: parent
    }
    Rectangle{
        id: circle
        border{
            width: 5
            color: "#44103558"
        }
        width: (Screen.width / 2) - 20
        height: (Screen.width / 2) -20
        radius: 360
        color: "#2196f3"
        anchors.centerIn: parent

    Image {
        id: splashImage
        width: circle.width / 2
        height: circle.width / 2
        fillMode: Image.PreserveAspectFit
        source: "qrc:/logo-wordsOnly.png"
        anchors.centerIn: parent
        scale: .7
        Behavior on scale{
            ParallelAnimation{
                RotationAnimation{
                    target: splashImage
                    from: 0
                    to: 360
                    duration: 1200
                }
                NumberAnimation{
                    duration: 1200
                    from: .7
                    to: 1
                    loops: Animation.Infinite
                    easing.type: Easing.OutQuint
                }
            }
        }
        Component.onCompleted: scale = 1
    }

    }
    Timer {
        interval: timeoutInterval;
        running: true;
        repeat: false
        onTriggered: {
            visible = false

            splash.timeout()
            splashWindow.destroy();
        }
    }
    Component.onCompleted: visible = true
}
