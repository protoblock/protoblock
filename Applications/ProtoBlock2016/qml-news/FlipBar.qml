
import QtQuick 2.0

Item {
    id: container
    property int animDuration: 300
    property Item front: Item {}
    property Item back: Item {}
    property real factor: 0.1 // amount the edges fold in for the 3D effect
    property alias delta: effect.delta
    property Item cur: frontShown ? front : back
    property Item noncur: frontShown ? back : front

    function swap() {
        var tmp = front;
        front = back;
        back = tmp;
        resync();
    }

    width: cur.width
    height: cur.height
    onFrontChanged: resync();
    onBackChanged: resync();

    property bool frontShown: true

    function resync() {//TODO: Are the items ever actually visible?
        back.parent = container;
        front.parent = container;
        if (frontShown) back.visible = false; else front.visible = false;
    }


    onFrontShownChanged: {
        back.visible = !frontShown
        front.visible = frontShown
    }

    function flipUp(start) {
        effect.visible = true;
        effect.sourceA = effect.source1
        effect.sourceB = effect.source2
        if (start == undefined)
            start = 1.0;
        deltaAnim.from = start;
        deltaAnim.to = 0.0
        dAnim.start();
        frontShown = false;
    }

    function flipDown(start) {
        effect.visible = true;
        effect.sourceA = effect.source1
        effect.sourceB = effect.source2
        if (start == undefined)
            start = 0.0;
        deltaAnim.from = start;
        deltaAnim.to = 1.0
        dAnim.start();
        frontShown = true;
    }

    ShaderEffect {
        id: effect
        width: cur.width
        height: cur.height
        property real factor: container.factor * width
        property real delta: 1.0

        mesh: GridMesh { resolution: Qt.size(8,2) }

        SequentialAnimation on delta {
            id: dAnim
            running: false
            NumberAnimation {
            id: deltaAnim
            duration: animDuration//expose anim
            }
        }

        property variant sourceA: source1
        property variant sourceB: source1
        property variant source1: ShaderEffectSource {
            sourceItem: front
            hideSource: effect.visible
        }

        property variant source2: ShaderEffectSource {
            sourceItem: back
            hideSource: effect.visible
        }

        fragmentShader: "
            uniform lowp float qt_Opacity;
            uniform sampler2D sourceA;
            uniform sampler2D sourceB;
            uniform highp float delta;
            varying highp vec2 qt_TexCoord0;
            void main() {
                highp vec4 tex = vec4(qt_TexCoord0.x, qt_TexCoord0.y * 2.0, qt_TexCoord0.x, (qt_TexCoord0.y-0.5) * 2.0);
                highp float shade = clamp(delta*2.0, 0.5, 1.0);
                highp vec4 col;
                if (qt_TexCoord0.y < 0.5) {
                    col = texture2D(sourceA, tex.xy) * (shade);
                } else {
                    col = texture2D(sourceB, tex.zw) * (1.5 - shade);
                    col.w = 1.0;
                }
                gl_FragColor = col * qt_Opacity;
            }
        "
        property real h: height
        vertexShader: "
        uniform highp float delta;
        uniform highp float factor;
        uniform highp float h;
        uniform highp mat4 qt_Matrix;
        attribute highp vec4 qt_Vertex;
        attribute highp vec2 qt_MultiTexCoord0;
        varying highp vec2 qt_TexCoord0;
        void main() {
            highp vec4 pos = qt_Vertex;
            if (qt_MultiTexCoord0.y == 0.0)
                pos.x += factor * (1. - delta) * (qt_MultiTexCoord0.x * -2.0 + 1.0);
            else if (qt_MultiTexCoord0.y == 1.0)
                pos.x += factor * (delta) * (qt_MultiTexCoord0.x * -2.0 + 1.0);
            else
                pos.y = delta * h;
            gl_Position = qt_Matrix * pos;
            qt_TexCoord0 = qt_MultiTexCoord0;
        }"
    }
    states: [
        State {
            name: "State1"
        }
    ]
}
