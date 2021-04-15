import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id: rectangle
    implicitWidth: 10
    implicitHeight: 64
    clip: true
    border.width: 1
    color: "#ffffff"

    Button{
        width: 20
        height: width+6
        text: "o"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 8
        anchors.topMargin: 0
        onClicked: {
            ahpItemMenu.indicatorName=indicatorName;
            ahpItemMenu.ahpKey=ahpKey;
            ahpItemMenu.open()
        }
    }

    Button{
        width: 20
        height: width+6
        text: "×"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.topMargin: 0
        onClicked: ahpModel.removeItem(ahpKey)
    }

    Rectangle{
        width: parent.width-16
        height: 30
        border.width: 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#e8ecef"
        radius: 4
        clip: true
        TextInput{
            width: parent.width-8
            clip: true
            anchors.centerIn: parent
            text:tabledata
            onTextChanged: {
                if(text!="" && tabledata!=text){
                    tabledata=text
                }
            }
        }
    }
}
