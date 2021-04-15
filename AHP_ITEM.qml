import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQml.Models 2.2

Dialog {
    id: dialog
    width: 340//(parent.width/2)
    height:(parent.height/4)*3
    anchors.centerIn: parent

    property string indicatorName: ""
    property string ahpKey: ""

    signal valuesChanged(string indname, string key, variant vals);
    signal positiveChanged(string indname, string key, bool pos);

    onOpened: {
        positiveSW.checked=false;
        valuesModel.clear();
        positiveSW.checked=CSI_Analysis.get_AHP_item_positive(indicatorName,ahpKey);
        var vobj =CSI_Analysis.get_AHP_item_values(indicatorName,ahpKey);
        for(var a in vobj)
        {
            valuesModel.append({"name":vobj[a]["name"],"value":vobj[a]["value"]});
        }
    }

    onClosed: {
        indicatorName="";
        ahpKey="";
        positiveSW.checked=false;
        valuesModel.clear();
    }

    ListModel {
        id: valuesModel
    }

    function creatNewList()
    {
        var valuesObj=({});
            for(var i = 0; i < visualModel.items.count; i++){
                var  innerObj=({});
                innerObj["name"]=visualModel.items.get(i).model.name;
                innerObj["value"]=visualModel.items.get(i).model.value;
                valuesObj[i+1]=innerObj;
            }
            valuesChanged(indicatorName,ahpKey,valuesObj);
    }

    Item {
        id:topItem
        width: parent.width
        height: 40
        anchors.top: parent.top

        Row{
            spacing: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30
            Switch{
                id:positiveSW
                anchors.verticalCenter: parent.verticalCenter
                hoverEnabled: true
                ToolTip.text: positiveSW.checked? "+":"-"
                ToolTip.visible: hovered
                onCheckedChanged: positiveChanged(indicatorName,ahpKey,checked)
            }

            Text {
                text: positiveSW.checked? "positive":"negative"
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
                color: positiveSW.enabled?"#000000":"#cecece"
            }

            Text {
                text: "تاثیر:"
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 12
            }
        }
    }

    ListView{
        id:listv0
        width: parent.width-32
        //height: contentHeight
        anchors.bottom: parent.bottom
        anchors.top: topItem.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        interactive: (contentHeight>(height+10))
        moveDisplaced: Transition {
                        NumberAnimation{
                            properties: "x,y"
                            duration: 200
                        }
                    }

        model:visualModel


        header:Item {
            width: listv0.width
            height: 40

            Text {
                text: "مقدار"
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 48
                font.pointSize: 12
            }

            Text {
                text: "نام"
                anchors.right: parent.right
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 36
                font.pointSize: 12
            }

            Rectangle{
                width: parent.width
                height: 2
                color: "#cecece"
                anchors.bottom: parent.bottom
            }
        }

        footer:Item {
            width: listv0.width
            height: 40

            Rectangle{
                width: parent.width
                height: 2
                color: "#cecece"
                anchors.top: parent.top
            }

            TextField {
                id:valueInp
                placeholderText: "مقدار"
                width: 40
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 48
                font.pointSize: 12
                selectByMouse: true
                validator: IntValidator{bottom: 1;top:99}
            }

            TextField {
                id:nameInp
                placeholderText: "نام"
                width: 120
                anchors.right: parent.right
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 30
                font.pointSize: 12
                selectByMouse: true
            }

            RoundButton {
                width: 40
                height: 30
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
                enabled: (nameInp.text.trim()!="" && valueInp.text.trim()!="")
                onClicked: {
                    valuesModel.append({"name":nameInp.text.trim(),"value":parseInt(valueInp.text.trim())});
                    nameInp.text="";
                    valueInp.text="";
                    creatNewList();
                }
                Text {
                    text: "+"
                    anchors.centerIn: parent
                }
            }


        }

        DelegateModel {
           id: visualModel
           model: valuesModel
           delegate: Item{
               id: dragArea
               width: listv0.width
               height: 40
               property bool held: false

               Rectangle{
                 id: content
                 anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                 width: dragArea.width
                 height: dragArea.height
                 opacity: dragArea.held ? 0.8 : 1.0

                 Drag.active: dragArea.held
                 Drag.source: dragArea
                 Drag.hotSpot.y: height / 2

                 Text {
                     id:valueTxt
                     text: value
                     anchors.left: delBTN.right
                     horizontalAlignment: Text.AlignHCenter
                     anchors.verticalCenter: parent.verticalCenter
                     anchors.leftMargin: 26
                 }

                 Text {
                     id:nameTxt
                     text: name
                     anchors.right: knob.left
                     horizontalAlignment: Text.AlignHCenter
                     anchors.verticalCenter: parent.verticalCenter
                     anchors.rightMargin: 0
                 }

                 RoundButton {
                     id:delBTN
                     width: 40
                     height: 30
                     anchors.left: parent.left
                     anchors.leftMargin: 0
                     anchors.verticalCenter: parent.verticalCenter
                     onClicked: {
                         valuesModel.remove(index)
                         creatNewList();
                     }
                     Text {
                         text: "×"
                         anchors.centerIn: parent
                     }
                 }

                 MouseArea {
                     id:knob
                     width: 40
                     height: 30
                     anchors.right: parent.right
                     anchors.rightMargin: 0
                     anchors.verticalCenter: parent.verticalCenter
                     drag.target: held ? content : undefined
                     drag.axis: Drag.YAxis
                     onPressed: held = true
                     onReleased: {
                         dragArea.held = false;
                         creatNewList();
                     }

                     Text {
                         text: "="
                         anchors.centerIn: parent
                     }
                 }


                 states: State{
                             when: dragArea.held
                             ParentChange { target: content; parent: listv0 }
                             AnchorChanges {
                                 target: content
                                 anchors { horizontalCenter: undefined; verticalCenter: undefined }
                             }
                         }
               }

               DropArea {
                   anchors.fill: parent
                   onEntered: {
                       visualModel.items.move( drag.source.DelegateModel.itemsIndex, dragArea.DelegateModel.itemsIndex);
                   }

               }

           }

        }
    }
}
