import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
//import QtQuick.Controls.Styles 1.4

Item {
    height: 240
    property int textPointSize: 9
    property int buttonWidth: 80
    property int buttonHeight: 20

    /*onVisibleChanged: { // if after change btn 'apply' not pressed
        if(isSettingsShow === false){
            editTextNote.text = dataBase.note
            slider.value = timer.maxSecPauseDuration
        }
    }*/

    // Label Info
    Text {
        id: txtLabel
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 16
        text: "Settings:"
    }

    //=============================
    // Symbols Settings
    //=============================
    GroupBox {
        id: groupBoxSymbols
        anchors.top: txtLabel.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: gridCheckBox.height
        //title: qsTr("Settings")
        Grid {
            id: gridCheckBox
            x: -12
            y: -11
            columns: 3
            rows: 2
            CheckBox {
                text: qsTr("Curly bracket { }")
                checked: symbolsSettings.curlyBracket
                onCheckStateChanged: {
                    symbolsSettings.curlyBracket = this.checked;
                }
            }
            CheckBox {
                text: qsTr("Parentheses ( )")
                checked: symbolsSettings.parentheses
                onCheckStateChanged: {
                    symbolsSettings.parentheses = this.checked;
                }
            }
            CheckBox {
                text: qsTr("Angle brackets < >")
                checked: symbolsSettings.angleBracket
                onCheckStateChanged: {
                    symbolsSettings.angleBracket = this.checked;
                }
            }
            CheckBox {
                text: qsTr("Asterisk *")
                checked: symbolsSettings.asterisk
                onCheckStateChanged: {
                    symbolsSettings.asterisk = this.checked;
                }
            }
            CheckBox {
                text: qsTr("Semicolon ;")
                checked: symbolsSettings.semicolon
                onCheckStateChanged: {
                    symbolsSettings.semicolon = this.checked;
                }
            }
            CheckBox {
                text: qsTr("Octothorpe #")
                checked: symbolsSettings.octothorpe
                onCheckStateChanged: {
                    symbolsSettings.octothorpe = this.checked;
                }
            }
        } // end grid
    } // end GroupBox of symbols

    Item {
        id: item2
        width: 500
        height: 120
        anchors.top: groupBoxSymbols.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10


        Column {
            id: columnLabelAndSlider
            spacing: 4
            Text {
                id: labelSliderInfo
                height: 20
                font.pointSize: textPointSize
                text: "Break time between key symbols which detected (seconds): "
            }

            //=============================
            // Time break Slider
            Slider {
                id: slider
                width: parent.width
                height: 20
                focusPolicy: Qt.WheelFocus
                spacing: 1
                from: 1
                to: 3600
                stepSize: 10

                background: Rectangle{
                    x: slider.leftPadding
                    y: slider.topPadding
                    implicitWidth: 200
                    implicitHeight: 4
                    width: slider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "lightgray"
                    Rectangle {
                        width: slider.visualPosition * parent.width
                        height: parent.height
                        color: timer.isTimerWorking ? "red" : "brown"
                        radius: 2
                    }
                }
                onValueChanged: {
                    txtSlider.text = value
                }
                Component.onCompleted: {
                    slider.value = timer.maxSecPauseDuration
                }
            } // end Slider

            //=============================
            // Note for statistics
            Text {
                id: labelNoteInfo
                height: 20
                font.pointSize: textPointSize
                text: "Note for future records on current session:"
            }
            Rectangle {
                width: labelSliderInfo.width
                height: buttonHeight
                color: "white"
                TextEdit {
                    id: editTextNote
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12

                    onTextChanged: {
                        if(dataBase.note !== editTextNote.getText(0, editTextNote.length)){
                            btnNoteApply.enabled = true;
                        } else {
                            btnNoteApply.enabled = false;
                        }
                    }
                    Component.onCompleted: {
                        editTextNote.text = dataBase.note
                    }
                }
            }
            Text {
                id: labelLogInfo
                height: 20
                font.pointSize: textPointSize
                text: "Log file: " + protocolLog.logFileName
            }
        } // end columns info

        Grid {
            id: gridButtonsSlider
            anchors.right: parent.right
            columns: 2
            columnSpacing: 4
            rowSpacing: 4
            //=============================
            // Time break Slider Buttons
            Button {
                id: btnSliderDefault
                width: buttonWidth
                height: buttonHeight
                text: "Default"
                hoverEnabled: true
                onClicked: {
                    slider.value = 300
                    timer.maxSecPauseDuration = slider.value
                }
            }
            Button {
                id: btnSliderApply
                width: buttonWidth
                height: buttonHeight
                text: "Apply"
                hoverEnabled: true
                enabled: {
                    (timer.maxSecPauseDuration === slider.value) ? false : true
                }
                onClicked: {
                    timer.maxSecPauseDuration = slider.value
                    txtSlider.text = timer.maxSecPauseDuration
                }
            }
            // end btn: slider
            //=============================
            // Slider text edit field
            Rectangle {
                id: editTextRectSlider
                width: buttonWidth
                height: buttonHeight
                color: "white"
                TextInput {
                    id: txtSlider
                    width: parent.width
                    height: parent.height
                    font.pixelSize: 12
                    maximumLength: 4
                    selectByMouse: true
                    mouseSelectionMode: TextInput.SelectCharacters
                    verticalAlignment: TextInput.AlignVCenter
                    validator: IntValidator{ bottom: 1; top: 3600; }
                    onTextChanged: {
                        slider.value = parseFloat(txtSlider.getText(0, 5))
                    }
                }
                KeyNavigation.tab: txtSlider
            } // end text input area for slider

            Repeater { // empty btn
                model: 3
                Rectangle {
                    width: buttonWidth
                    height: buttonHeight
                    color: "transparent"
                }
            }
            //=============================
            // Note buttons
            Button {
                id: btnNoteClear
                width: buttonWidth
                height: buttonHeight
                text: "Clear"
                hoverEnabled: true
                onClicked: {
                    editTextNote.clear();
                    editTextNote.focus = true;
                }
            }
            Button {
                id: btnNoteApply
                width: buttonWidth
                height: buttonHeight
                text: "Apply"
                hoverEnabled: true
                enabled: false
                onClicked: {
                    dataBase.note = editTextNote.text;
                    btnNoteApply.enabled = false
                }
            }
            //=============================
            // Log buttons
            Button {
                width: buttonWidth
                height: buttonHeight
                text: "Open Log"
                hoverEnabled: true
                onClicked: {
                    protocolLog.openLogFileFolder();
                }
            }
            Button {
                width: buttonWidth
                height: buttonHeight
                text: "Remove Log"
                hoverEnabled: true
                onClicked: {
                    dialogRemoveLog.open()
                }
            }
        } // end grid buttons
    } // end item
    MessageDialog{
        id: dialogRemoveLog
        title: qsTr("Remove Log file")
        text: qsTr("Do you confirm remove log file?")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            protocolLog.removeCurrentLogFile();
        }
    } // end MessageDialog
}
