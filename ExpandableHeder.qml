import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
                implicitWidth: 10
                implicitHeight: 64
                clip: true
                color: "#e8ecef"

                Text {
                    id:weight0
                    text: positive
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.top: parent.top
                }
                Text {
                    text: weight
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    HoverHandler{
                        id:hh
                    }
                    ToolTip.text: "وزن"
                    ToolTip.visible: hh.hovered
                }

                Text {
                    text: tabledata
                    color: "#000000"
                    anchors.centerIn: parent
                    font.pointSize: 12
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                }
                Row{
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.bottom: parent.bottom

                    RippleButton{
                        text: "\u25b4"
                        color: "#e8ecef"
                        border.width: 0
                        width: expandMode===2?(parent.width-28):14;
                        height: 24
                        textVerticalCenterOffset:-2
                        hoverEnabled: true
                        ToolTip.text: "میانگین موزون"
                        ToolTip.visible: hovered
                        ToolTip.delay: 1000
                        Behavior on width{NumberAnimation{}}
                        Rectangle{
                            width: parent.width
                            height: 4
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#545454"
                        }
                        onClicked: swa_model.setExpandMode(column,2)
                    }
                    RippleButton{
                        text: "\u25b4"
                        color: "#e8ecef"
                        border.width: 0
                        width: expandMode===1?(parent.width-28):14
                        height: 24
                        textVerticalCenterOffset:-2
                        hoverEnabled: true
                        ToolTip.text: "ضریب اهمیت"
                        ToolTip.visible: hovered
                        ToolTip.delay: 1000
                        Behavior on width{NumberAnimation{}}
                        Rectangle{
                            width: parent.width
                            height: 4
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#7c7c7c"
                        }
                        onClicked: swa_model.setExpandMode(column,1)
                    }
                    RippleButton{
                        text: "\u25b4"
                        color: "#e8ecef"
                        border.width: 0
                        width: expandMode===0?(parent.width-28):14
                        height: 24
                        textVerticalCenterOffset:-2
                        hoverEnabled: true
                        ToolTip.text: "اهمیت"
                        ToolTip.visible: hovered
                        ToolTip.delay: 1000
                        Behavior on width{NumberAnimation{}}
                        Rectangle{
                            width: parent.width
                            height: 4
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#a7a7a7"
                        }
                        onClicked: swa_model.setExpandMode(column,0)
                    }
                }
                MouseArea{
                    enabled: columnIndex!==0
                    visible: enabled
                    height: parent.height
                    width: 2
                    anchors.left: parent.left
                    drag.target: this
                    drag.axis: Drag.XAxis
                    cursorShape: Qt.SizeHorCursor;
                    onPositionChanged: {
                        if(pressed)
                        {
                            var newWidth=tableView.columnWidths[columnIndex]-mouseX;
                            var lastCellNewWidth=tableView.columnWidths[columnIndex-1]+mouseX;

                            if( (newWidth>8 && mouseX>0)  ||  (lastCellNewWidth>8 && mouseX<0) ){
                                    tableView.columnWidths[columnIndex-1] = lastCellNewWidth
                                    tableView.columnWidths[columnIndex] = newWidth
                            }
                            tableView.forceLayout();
                        }
                    }

                    Rectangle{width: 1;height: parent.height-16;color:"#cecece";anchors.verticalCenter: parent.verticalCenter;anchors.left: parent.left;}
                }
                MouseArea{
                    enabled: rowIndex!==0
                    visible: enabled
                    height: 2
                    width: parent.width
                    anchors.top: parent.top
                    drag.target: this
                    drag.axis: Drag.YAxis
                    cursorShape: Qt.SizeVerCursor;
                    onPositionChanged: {
                        if(pressed)
                        {
                            var newHeight=tableView.rowHeights[rowIndex]-mouseY;
                            var lastCellNewHeight=tableView.rowHeights[rowIndex-1]+mouseY;

                            if( (newHeight>8 && mouseY>0)  ||  (lastCellNewHeight>8 && mouseY<0) ){
                                    tableView.rowHeights[rowIndex-1] = lastCellNewHeight
                                    tableView.rowHeights[rowIndex] = newHeight
                            }
                            tableView.forceLayout();
                        }
                    }
                    Rectangle{
                        width: parent.width;height: 1;color:"#cecece";anchors.bottom: parent.bottom;
                    }
                }
            }
