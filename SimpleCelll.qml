import QtQuick 2.12

Rectangle {
                implicitWidth: 10
                implicitHeight: 64
                clip: true
                color: maxScore?"#fff1a1":"#ffffff"
                Behavior on color{ColorAnimation{}}

                Text {
                    text: tabledata
                    color: "#545454"
                    anchors.centerIn: parent
                    font.pointSize: 11
                    horizontalAlignment: Text.AlignHCenter
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
