import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Rectangle{
    id:mainRect
    width: 200
    height: innerText.height+20
    color: "#ffffff"
    border.width: 1
    border.color: "#ffc501"
    radius: 4
    clip: true

    signal clicked();
    signal pressed();
    signal released();

    property int textVerticalCenterOffset:0;
    property alias hoverEnabled: button.hoverEnabled;
    property alias text: innerText.text;
    property alias fontPointSize: innerText.font.pointSize;
    property alias textColor: innerText.color;
    property color rippleColor: "#40FFFFFF";
    property bool hovered: false;

    Text{
        id:innerText
        horizontalAlignment: Text.AlignHCenter
        width: parent.width-32
        wrapMode: Text.WordWrap
        font.pointSize: 12
        anchors.centerIn: parent
        anchors.verticalCenterOffset:textVerticalCenterOffset
    }

    MouseArea{
        id: button
        anchors.fill: parent
        onPressed: {
            ripple.x=mouseX-(ripple.width/2);
            ripple.y=mouseY-(ripple.height/2);
            mainRect.pressed();
        }
        onClicked:mainRect.clicked();
        onReleased: mainRect.released();
        onEntered: hovered=true;
        onExited: hovered=false;

        Ripple {
            id: ripple
            clipRadius: 2
            //anchors.fill: mainRect
            x:40
            width: mainRect.width*2
            height: mainRect.height*2
            pressed: button.pressed
            active:  false
            color: rippleColor
            layer.enabled: true
        }
    }
}
