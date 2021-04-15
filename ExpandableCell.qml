import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    implicitWidth: 10
    implicitHeight: 64
    clip: true
    color: maxScore?"#fff1a1":"#ffffff";
    Behavior on color{ColorAnimation{}}


        Item{
            id:c1
            visible: expandMode===2;
            width: expandMode===2?(parent.width):0;
            height: parent.height
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            Behavior on width{NumberAnimation{}}
            Text {
                text: weightedAverage
                anchors.centerIn: parent
            }
        }

        Item{
            id:c2
            visible: expandMode===1;
            width: expandMode===1?(parent.width):0
            height: parent.height
            anchors.left: c1.right
            anchors.bottom: parent.bottom
            Behavior on width{NumberAnimation{}}
            Text {
                text: selectedSAWValue
                anchors.centerIn: parent
            }
        }

        ComboBox {
            id:combo
            property bool touched: false;
            background: Item{}
            visible: expandMode===0;
            font.pointSize: 11
            anchors.left: c2.right
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            delegate: ItemDelegate {
                    width: combo.width
                    contentItem: Text {
                        text: modelData
                        font: combo.font
                        color: "#545454"
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: combo.highlightedIndex === index
                }

            contentItem: Text {
                    leftPadding: 12
                    rightPadding: combo.indicator.width + combo.spacing

                    text: combo.displayText
                    font: combo.font
                    color: "#545454"
                    verticalAlignment: Text.AlignVCenter
                }

            Behavior on width{NumberAnimation{}}
            currentIndex: selectedSAW
            model: listdata
            onPressedChanged: {
                if(pressed)
                    touched=true;
            }
            onCurrentTextChanged: {
                if(currentIndex!==selectedSAW && touched){
                    swa_model.setSelected(ahpKey,rowIndex,columnIndex,currentIndex);
                }
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
