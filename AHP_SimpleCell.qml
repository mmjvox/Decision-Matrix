import QtQuick 2.12

Rectangle{
            implicitWidth: 10
            implicitHeight: 64
            clip: true
            border.width: 1
            color: diameter?"#e8ecef":"#ffffff"
            Text {
                id: name
                text: diameter?"1":tabledata
                anchors.centerIn: parent
            }
}
