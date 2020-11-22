import QtQuick 2.0
import QtQuick.Window 2.2

Item {
	id: item
	width: parent.width
	height: 30
	signal showDeleteRowDialog()

	//=============================
	// Rectangle of Button
	//=============================
	Rectangle {
		id: buttonRect
		width: parent.width
		height: parent.height
		color: "lightgray"
		visible: {
			if(buttonIndex == 4 || buttonIndex == 5){
				return isModelViewLoaded;
			} else {
				return true;
			}
		}
		//=============================
		Text {
			id: btnTxt
			text: buttonLabel
			anchors.centerIn: buttonRect
			font.pointSize: textPointSize
			color: "black"
		}
		//=============================
		MouseArea {
			anchors.fill: buttonRect
			hoverEnabled: true
			onEntered: buttonRect.color = "#e6e6e6"
			onExited: buttonRect.color = "lightgray"
			onClicked: {
				scaleAnimation.start();
				if(buttonIndex == 0){
					databaseModel.updateModel();
					showResult();
				}
				else if(buttonIndex > 0 && buttonIndex <= 3){
					isModelViewLoaded = false;
					loader.source = element;
				}
				else if(buttonIndex == 4) {
					//-----------------------------------------------------------
				}
				else if(buttonIndex == 5)
					dialogRemoveAll.open();

			}
		}
		//=============================
		// click animation
		PropertyAnimation {
			id: scaleAnimation
			target: btnTxt
			property: "font.pointSize"
			from: textPointSize+1
			to: textPointSize
			duration: 500
			easing {
				type: Easing.OutBack
				overshoot: 5
			}
		}
	}// end Rectangle of button
}
