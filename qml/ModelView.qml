import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Item {
    id: modelView
    height: window.height - 125

    property string currentNote;

    //=============================
    // Table of Statistic
    //=============================
    TableView {
        id: tableView
        width: parent.width
        height: parent.height
        verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
        model: databaseModel

        TableViewColumn {
            id: idColumn
            role: "id"
            title: "ID"
            width: 30
            visible: true
            resizable: false
        }
        TableViewColumn{
            id: dateColumn
            role: "date"
            title: "Date"
            width: 65
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                    //font.pixelSize: 14
                }
            }
        }
        TableViewColumn{
            id: startTimeColumn
            role: "startSession"
            title: "Start"
            width: 55
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }
        TableViewColumn{
            id: endTimeColumn
            role: "endSession"
            title: "End"
            width: 55
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }
        TableViewColumn{
            id: workTimeColumn
            role: "workTime"
            title: "Work hours"
            width: 70
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }        
        TableViewColumn{
            id: writingCodeTimeColumn
            role: "writingCodeTime"
            title: "Writing code"
            width: 70
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }
        TableViewColumn{
            id: workSecondsColumn
            role: "workSecs"
            title: "Working seconds"
            width: 50
            visible: false
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }

        TableViewColumn{
            id: writingCodeSecondsColumn
            role: "writingCodeSecs"
            title: "Writing Code seconds"
            width: 50
            visible: false
            delegate: Item {
                anchors.fill: parent
                Text {
                    text: styleData.value
                }
            }
        }
        //comments
        TableViewColumn{
            id: commentsColumn
            role: "comments"
            title: "Note"
            width: 115
            visible: true
            delegate: Item {
                anchors.fill: parent
                clip: true
                Text {
                    text: styleData.value
                }
            }
        }

        rowDelegate: Rectangle {
            anchors.fill: parent
            color: styleData.selected ? "skyblue" : (styleData.alternate ? "whitesmoke" : "white");
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: {
                    tableView.selection.clear()
                    tableView.selection.select(styleData.row)
                    tableView.currentRow = styleData.row
                    tableView.focus = true
                    switch(mouse.button){
                        case Qt.RightButton:
                            contextMenu.popup();
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    } // end TableView

    //=============================
    // Context Menu
    //=============================
    Menu {
        id: contextMenu
        MenuItem {
            id: contextEditNote
            text: qsTr("Edit note...")
            onTriggered: {
                currentNote = dataBase.getNoteById(databaseModel.getID(tableView.currentRow));
                dialogEditNote.open();
            }
        }
        MenuItem {
            id: contextDeleteRow
            text: qsTr("Remove row")
            onTriggered: {
                dialogRemoveRow.open();
            }
        }        
    }
    //=============================
    // Edit Note Dialog
    Dialog {
        id: dialogEditNote
        Text {
            id: dialogEditTxtInfo
            font.pointSize: textPointSize
            text: "Edit note:"
        }
        Rectangle {
            id: dialogEditTxtRect
            width: 250
            height: 20
            anchors.top: dialogEditTxtInfo.bottom
            anchors.topMargin: 10
            color: "white"

            TextInput {
                id: dialogEditTxtInput
                width: parent.width
                height: parent.height
                font.pixelSize: 13
                maximumLength: 30
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                verticalAlignment: TextInput.AlignVCenter
            }
        }

        standardButtons: StandardButton.Cancel | StandardButton.Ok
        onAccepted: {
            dataBase.editNote(databaseModel.getID(tableView.currentRow),
                                    dialogEditTxtInput.getText(0, dialogEditTxtInput.length));
            databaseModel.updateModelWithLastQuery();
        }
        onRejected: {
            //console.log("reject");
        }
        onVisibleChanged: {
            if(visible === true){
                dialogEditTxtInput.text = currentNote;
                dialogEditTxtInput.focus = true;
            }
        }
    } // end dialog edit note

    //=============================
    // Remove Dialog
    MessageDialog {
        id: dialogRemoveRow
        title: qsTr("Remove row")
        text: qsTr("Do you confirm to remove the selected row from Statistics?")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            dataBase.removeRecordById(databaseModel.getID(tableView.currentRow));
            databaseModel.updateModel();
        }
    }

}
