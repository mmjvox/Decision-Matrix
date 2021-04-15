import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtCharts 2.3


Window {
    id: window
    width: 900
    height: 600
    visible: true
    title: qsTr("Hello World")

    Loader{
        id:loader1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        source: "qrc:/DecisionMatrix.qml"
    }
}
