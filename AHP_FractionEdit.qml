import QtQuick 2.12

Rectangle{
            implicitWidth: 10
            implicitHeight: 64
            clip: true
            border.width: 1
            color: diameter?"#e8ecef":"#ffffff"

            Column{
                anchors.centerIn: parent
                spacing: 2

                Rectangle{
                    width: 40
                    height: 26
                    border.width: 2
                    border.color: "#e8ecef"
                    radius: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    TextInput{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator{bottom:1;top:100}
                        selectByMouse: true
                        text: numerator
                        onTextChanged: {
                            if(text!="" && numerator!=text){
                                numerator=text
                            }
                        }
                    }
                }

                Rectangle{
                    width: parent.parent.width-32
                    height: 2
                    color: "#000000"
                }

                Rectangle{
                    width: 40
                    height: 26
                    border.width: 2
                    border.color: "#e8ecef"
                    radius: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    TextInput{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator{bottom:1;top:100}
                        selectByMouse: true
                        text: denominator
                        onTextChanged: {
                            if(text!="" && denominator!=text){
                                denominator=text
                            }
                        }
                    }
                }
            }
}
