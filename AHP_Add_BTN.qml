import QtQuick 2.12

RippleButton{
            implicitWidth: 10
            implicitHeight: 64
            clip: true
            border.width: 1
            color: "#ffffff"
            border.color: "#000000"
            radius: 0
            rippleColor: "#50ffc501"
            text: "+"

            onClicked: addnewDialog.open()
}
