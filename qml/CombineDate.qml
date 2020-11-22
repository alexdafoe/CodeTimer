import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
Item {

	function loadNote() {
		databaseModel.clearSelectedIdList(); // clear idList after date changed
		var dateWithNote = new Date(boxSelectYear.getText(0, 5), boxSelectMonth.currentIndex -1, boxSelectDay.currentIndex);
		//console.log(dateWithNote);
		boxChooseNote.model = databaseModel.getDateNotesList(dateWithNote);
		boxNoteInput.text = boxChooseNote.currentText
	}

	Component.onCompleted: {
		boxSelectDay.currentIndex = Qt.formatDateTime(currentDate, "d");
		boxSelectMonth.currentIndex = Qt.formatDateTime(currentDate, "M");
		boxSelectYear.text = Qt.formatDateTime(currentDate, "yyyy");
	}

	Text {
		id: uniteDateInfo
		anchors.top: parent.top
		anchors.topMargin: 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		font.pointSize: textPointSize
		text: "To unite the date:"
	}
	Grid {
		id: gridSelectDate
		anchors.top: uniteDateInfo.bottom
		anchors.topMargin: 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		spacing: 8
		rows: 2
		columns: 3
		Text {
			id: selectDayTxt
			font.pointSize: textPointSize
			text: "Day"
		}
		Text {
			id: selectMonthTxt
			font.pointSize: textPointSize
			text: "Month"
		}
		Text {
			id: selectYearTxt
			font.pointSize: textPointSize
			text: "Year"
		}

		ComboBox {
			id: boxSelectDay
			width: 80
			height: 30
			model: 32
			validator: IntValidator{ bottom: 1; top: 31;}
			onCurrentIndexChanged: { // if 0
				if(boxSelectDay.currentIndex < 1) boxSelectDay.currentIndex = 1;
			}

			onActivated: {
				loadNote();
			}
		}
		ComboBox {
			id: boxSelectMonth
			width: 80
			height: 30
			model: 13
			validator: IntValidator{ bottom: 1; top: 12;}
			onCurrentIndexChanged: { // if 0
				if(boxSelectMonth.currentIndex < 1) boxSelectMonth.currentIndex = 1;
			}
			onActivated: {
				loadNote();
			}
		}
		Rectangle {
			id: rectSelectYearRect
			width: 80
			height: 30
			color: "white"

			TextInput {
				id: boxSelectYear
				width: parent.width
				height: parent.height
				color: "black"
				font.pixelSize: 12
				maximumLength: 4
				selectByMouse: true
				mouseSelectionMode: TextInput.SelectCharacters
				verticalAlignment: TextInput.AlignVCenter
				validator: IntValidator{ bottom: 2000; top: 2050;}
				onTextChanged: {
					loadNote();
				}
			}
		}
	} // end grid select date

	Grid {
		id: gridChooseNote
		anchors.top: gridSelectDate.bottom
		anchors.topMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 10
		width: gridSelectDate.width
		rows: 4
		columns: 1
		spacing: 5

		Text {
			id: noteBoxInfo
			font.pointSize: textPointSize
			text: databaseModel.dateHasNote
				  ? "This date has notes:"
				  : "This date hasn't notes yet."
		}

		ComboBox {
			id: boxChooseNote
			width: parent.width
			height: 30
			enabled: databaseModel.dateHasNote
			onActivated: {
				boxNoteInput.text = boxChooseNote.currentText
			}
		}

		Text {
			id: noteInputInfo
			font.pointSize: textPointSize
			text: (databaseModel.dateRowsCount > 0)
				  ? "Write the note:"
				  : "This date hasn't records."

			property string toolTipText: "Write the note or select above for unite"
			ToolTip.visible: (databaseModel.dateRowsCount > 0) ? tollTipNoteInputArea.containsMouse : false
			ToolTip.text: toolTipText
			MouseArea {
				id: tollTipNoteInputArea
				anchors.fill: parent
				hoverEnabled: true
			}
		}

		Rectangle {
			id: noteInputRect
			width: parent.width
			height: 20
			color: (databaseModel.dateRowsCount > 0)
				   ? "white"
				   : "lightgray"

			TextInput {
				id: boxNoteInput
				width: parent.width
				height: parent.height
				font.pixelSize: 12
				maximumLength: 30
				selectByMouse: true
				mouseSelectionMode: TextInput.SelectCharacters
				verticalAlignment: TextInput.AlignVCenter
				enabled: (databaseModel.dateRowsCount > 0)
				Component.onCompleted: {
					text = boxChooseNote.currentText
				}
			}
		}
	} // end grid choose note

	Button {
		width: 80
		height: 20
		anchors.top: gridChooseNote.bottom
		anchors.topMargin: 8
		anchors.left: parent.left
		anchors.leftMargin: 10
		text: "Select rows..."
		hoverEnabled: true
		enabled: (databaseModel.dateRowsCount > 2)
		onClicked: {
			dialogSelectionRow.open();
		}
	} // end button dialog selection row

	Button {
		id: btnOk
		width: 64
		height: 30
		anchors.top: parent.top
		anchors.topMargin: 5
		anchors.right: parent.right
		anchors.rightMargin: 10
		hoverEnabled: true
		font.pointSize: textPointSize
		text: "Ok"

		onClicked: {
			if(databaseModel.dateRowsCount > 1)
				dialogConfirm.open();
			else
				dialogNoRecordsInfo.open();
		}
	} // end button Ok

	Button {
		id: btnClear
		width: btnOk.width
		height: btnOk.height
		anchors.top: btnOk.bottom
		anchors.topMargin: 5
		anchors.right: parent.right
		anchors.rightMargin: 10
		font.pointSize: textPointSize
		hoverEnabled: true
		text: "Clear"
		onClicked: {
			boxNoteInput.clear();
			boxNoteInput.focus = true;
		}
	} // end button Clear



	MessageDialog {
		id: dialogConfirm
		title: qsTr("Unite date")
		text: qsTr("Do you confirm to unite the date with note \nspecified in the edit line?")
		icon: StandardIcon.NoIcon
		standardButtons: StandardButton.Ok | StandardButton.Cancel

		onAccepted: {
			var date = new Date(boxSelectYear.getText(0, 5),
								boxSelectMonth.currentIndex -1,
								boxSelectDay.currentIndex)
			dataBase.uniteDateWithNote(date, boxNoteInput.getText(0, boxNoteInput.length));
			databaseModel.updateModel();
			showResult();
		}
	}

	MessageDialog {
		id: dialogNoRecordsInfo
		title: qsTr("Unite date")
		text: qsTr("This date has less than 2 records, there is nothing to unite.")
		icon: StandardIcon.Information
		standardButtons: StandardButton.Ok
	}


	Dialog {
		id: dialogSelectionRow
		width: 405
		standardButtons: StandardButton.Cancel | StandardButton.Ok
		onVisibilityChanged: {
			if(dialogSelectionRow.visible){
				databaseModel.clearSelectedIdList();
				tableViewSelection.selection.forEach( function(rowIndex) {
					tableViewSelection.selection.deselect(rowIndex);
				}
				);
			}
		}

		onAccepted: {
			tableViewSelection.selection.forEach( function(rowIndex) {
				databaseModel.addSelectedIdToList(databaseModel.getID(rowIndex));
			}
			);
		}

		TableView {
			id: tableViewSelection
			width: 380

			verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
			selectionMode: SelectionMode.ExtendedSelection
			model: databaseModel

			TableViewColumn {
				id: idColumn
				role: "id"
				title: "ID"
				width: 30
				visible: true
			}
			TableViewColumn{
				id: dateColumn
				role: "date"
				title: "Date"
				width: 65
				delegate: Item {
					anchors.fill: parent
					Text {
						text: styleData.value
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
					Text {
						text: styleData.value
					}
				}
			}
			TableViewColumn{
				id: commentsColumn
				role: "comments"
				title: "Note"
				width: 150
				visible: true
				delegate: Item {
					anchors.fill: parent
					Text {
						text: styleData.value
					}
				}
			}

			rowDelegate: Rectangle {
				anchors.fill: parent
				color: styleData.selected ? "skyblue" : (styleData.alternate ? "whitesmoke" : "white");
			} // end rowDelegate TableView

		} // end TableView Dialog

	} // end Dialog Selection Row

}
