import QtQuick 2.0

Item {
	//width: 640 //need comment parameter
	height: 30
	property int buttonTextPointSize: 11

	Rectangle {
		id: mainLineRect
		anchors.fill: parent
		color: "lightgray"
	}

	ListView{
		id: advancedButtons
		width: parent.width
		height: parent.height
		spacing: parent.width / 5
		orientation: ListView.Horizontal
		interactive: false

		model: ListModel {
			ListElement {
				buttonLabel: "Settings"
				buttonIndex: 0
			}
			ListElement {
				buttonLabel: "Dial"
				buttonIndex: 1
			}
			ListElement {
				buttonLabel: "Statistics"
				buttonIndex: 2
			}
		} // end model ListModel

		//=============================
		// Button Rectangle
		delegate: Rectangle {
			id: buttonRect
			width: parent.width / 5
			height: parent.height
			color: "#e6e6e6"
			//=============================
			Text {
				id: btnTxt
				text: buttonLabel
				anchors.centerIn: buttonRect
				font.pointSize: buttonTextPointSize
				color: "black"
			}
			//=============================
			MouseArea {
				anchors.fill: buttonRect
				hoverEnabled: true
				onEntered: buttonRect.color = "darkgray"
				onExited: buttonRect.color = "#e6e6e6";
				onClicked: {
					swipeView.currentIndex = buttonIndex;
					if(buttonIndex == 2) databaseModel.update();
				}
			}
			//=============================
			PropertyAnimation {
				id: scaleAnimation
				target: btnTxt
				property: "font.pointSize"
				from: buttonTextPointSize+3
				to: buttonTextPointSize
				duration: 500
				easing {
					type: Easing.OutBack
					overshoot: 5
				}
			}
		} // end delegate Rectangle
	} // end ListView

}
