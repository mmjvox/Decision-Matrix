import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQml.Models 2.2

Dialog {
    id: dialog
    width: 340//(parent.width/2)
    height:200
    anchors.centerIn: parent

    TextField{
        id: newOne
        placeholderText: "نام مورد جدید"
        anchors.centerIn: parent
        width: 160
    }

    Button{
            text: "تایید"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onClicked: ahpModel.addNewOne(newOne.text);
    }
}
