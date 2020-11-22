import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
	Component.onCompleted: searchNoteEditLine.focus = true;

	//=============================
	// Search Note
	//=============================
	Text {
		id: searchNoteTxt
		anchors.top: parent.top
		anchors.topMargin: 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		font.pointSize: textPointSize
		text: "Search of the note:"
	}

	Rectangle {
		id: searchNoteEditRect
		height: 20

		anchors.top: parent.top
		anchors.topMargin: 5
		anchors.right: btnOk.left
		anchors.rightMargin: 10
		anchors.left: searchNoteTxt.right
		anchors.leftMargin: 10
		color: "white"

		TextInput {
			id: searchNoteEditLine
			width: parent.width
			height: parent.height
			text: qsTr("write note here")
			cursorVisible: true
			color: "black"
			font.pointSize: textPointSize
			maximumLength: 30
			selectByMouse: true
			mouseSelectionMode: TextInput.SelectCharacters
			verticalAlignment: TextInput.AlignVCenter
		}
	}
	CheckBox {
		id: boxSimilar
		text: qsTr("similar beginning")
		anchors.top: searchNoteEditRect.bottom
		anchors.left: searchNoteEditRect.left
		anchors.leftMargin: -8
		checked: false
	}

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
			databaseModel.searhNote(searchNoteEditLine.getText(0, searchNoteEditLine.length),
										 boxSimilar.checkState);
			showResult();
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
			searchNoteEditLine.clear();
			searchNoteEditLine.focus = true;
		}
	} // end button Clear
}
