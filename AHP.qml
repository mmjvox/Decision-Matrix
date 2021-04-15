import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import AHPModel 0.1

Dialog {
    id:ahpDialog
    width: (parent.width/ 8)*7
    height:(parent.height/8)*7
    anchors.centerIn: parent

    property alias ahpModel: ahpModel
    property alias indicatorName: ahpModel.indicatorName

//    onWidthChanged: ahpTable.forceLayout();
//    onHeightChanged: ahpTable.forceLayout();

    onOpened: {
        ahpModel.ahpObject=CSI_Analysis.get_AHP(indicatorName);
    }

    AHP_Add_New{
        id:addnewDialog
    }

    AHPModel{
        id:ahpModel
        onAhpObjectChanged: CSI_Analysis.set_AHP(indicatorName,ahpObject);
    }



    contentItem:Item {
        id: item1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0

        Button{
            id: editBTN
            text:ahpModel.editMode?"Done":"Edite"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.rightMargin: 20
            highlighted: true
            Material.accent: ahpModel.editMode? Material.Green : "#ffc501"
            Material.foreground: ahpModel.editMode? "#ffffff" : "#000000"
            onClicked: {
                ahpModel.editMode=!ahpModel.editMode;
            }
        }

        Item{
            id:tableParent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: editBTN.bottom
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 6
            anchors.bottomMargin: 20

            TableView{
                id:ahpTable
                width: parent.width
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                interactive: false
                columnSpacing: 0
                rowSpacing: 0
                clip: true
                model: ahpModel

                columnWidthProvider: function (column) { return  (tableParent.width) / ahpModel.columnCount(); }
                rowHeightProvider: function (row) { return  row===0? 40 : ( (tableParent.height) / ahpModel.rowCount() ) ;   }

                delegate: Loader{
                    source:diameter?"qrc:/AHP_SimpleCell.qml":editModeCell
                }
            }
        }
    }
}
