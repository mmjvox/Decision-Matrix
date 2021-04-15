import QtQuick 2.12
import QtQuick.Controls 2.12
import TableModel 0.1

Item {

    Component.onCompleted: {
        parentModel.clear();
        CSI_Analysis.get_selectIndicators()
    }
    Connections{
        target: CSI_Analysis
        onAppendSelectedInds:{
                parentModel.append({"indicatorName1":ind})
        }
    }

//    AHP_ITEM {
//        id:ahpItemMenu
//        onValuesChanged: ahpMenu.ahpModel.set_AHP_values(indname, key, vals);
//        onPositiveChanged: ahpMenu.ahpModel.set_AHP_positive(indname, key, pos);
//        //Component.onCompleted: open()
//    }

//    AHP {
//        id:ahpMenu
//        indicatorName: "Quality of repairs"//indicatorName1
////        Component.onCompleted: open()
////        onOpened: ahpItemMenu.open()
//    }

    ListView{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        model: ListModel{
            id:parentModel
        }

        delegate:    Column{
            width: parent.width

            Rectangle{
               id:headerRect
               width: parent.width
               height: 40
               clip: true
               color: "#ffc501"
               onWidthChanged: tableView.resetWH();
               onHeightChanged: tableView.resetWH();

               RoundButton{
                   text: ""
                   anchors.verticalCenter: parent.verticalCenter
                   //onClicked: ahpMenu.open()
               }

               Text {
                   text: indicatorName1
                   anchors.centerIn: parent
                   font.pointSize: 14
               }
            }

            TableView {
                id:tableView
                width: parent.width
                height: contentHeight
                interactive: false
                columnSpacing: 0
                rowSpacing: 0
                clip: true

                Component.onCompleted: {
                    swa_model.clustersObject=CSI_Analysis.get_cluster( indicatorName1 );
                    swa_model.ahpObject=CSI_Analysis.get_AHP( indicatorName1 );
                    resetWH();
                }

                function resetWH()
                {
                    console.log("resetWH")
                    tableView.presetWidths();
                    tableView.presetHeights();
                    tableView.forceLayout();
                }

                property variant columnWidths: [];
                property variant rowHeights: [];

                function presetWidths(){
                    var cc=swa_model.columnCount();
                    columnWidths[0]=60;
                    for(var i=1; i<cc; i++){
                        if(i===cc-2)
                            columnWidths[i]=400;
                        else if(i===cc-1)
                            columnWidths[i]=200;
                        else{
                            columnWidths[i]=(tableView.width-660)/(cc-3)
                        }
                    }
                }
                function presetHeights(){
                    console.log("H")
                    var rr=swa_model.rowCount();
                    rowHeights[0]=80;
                    for(var i=1; i<rr; i++){  rowHeights[i]=40; }
                }

                columnWidthProvider: function (column){
                    return columnWidths[column]>0?columnWidths[column]:60
                }
                rowHeightProvider: function (row) {
                    return rowHeights[row]>0?rowHeights[row]:60
                }

                model: TableModel {
                    id:swa_model
                    onClustersObjectEdited:{
                        CSI_Analysis.set_Clusters( indicatorName1 , clustersObject);
                    }
                    onExportMaxData:{
                        CSI_Analysis.set_Correction( indicatorName1 , score , clusterName , level , levelName );
                    }
                }

                delegate: Loader{
                    source: switch(heading) {
                                    case 0: return "qrc:/SimpleHeader.qml"
                                    case 1: return "qrc:/ExpandableHeder.qml"
                                    case 2: return "qrc:/SimpleCelll.qml"
                                    case 3: return "qrc:/ExpandableCell.qml"
                                }
                }
            }
        }
    }

}
