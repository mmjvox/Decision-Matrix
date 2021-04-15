import QtQuick 2.12

Rectangle{
    width: 20
    height: 20
//    anchors.left: parent.left
//    anchors.leftMargin: 40
    id:control
    property alias text:tx.text;
    property int verticalCenterOffset:0;
    property alias button:button
    property alias fontFamily: tx.font.family;
    property alias fontPointSize: tx.font.pointSize;

    Text{
        id:tx
        anchors.verticalCenterOffset: verticalCenterOffset
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        antialiasing: true;
    }

    MouseArea{
        anchors.fill: parent
        id:button
    }
}
