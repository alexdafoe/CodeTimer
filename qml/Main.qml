import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2 as QtC22
ApplicationWindow {
	id: window
	visible: true
	// fixed size to prevent error:
	// D3D11 ERROR ...
	// [ STATE_SETTING ERROR #260: DEVICE_RSSETSCISSORRECTS_INVALIDSCISSOR]
	// generated SwipeView when make full screen window
	width: minimumWidth = maximumWidth = 640
	height: minimumHeight = maximumHeight = 340

	Connections {
		target: sysTray
		onSignalShow: {
			window.show()
		}
		onSignalQuit:{
			sysTray.hideIconTray();
			Qt.quit();
		}
		onSignalIconActivated: {
			window.visible ? window.hide() : window.show();
		}
	}
	onClosing: {
		window.hide();
	}

	Rectangle {
		id: topLine
		height: 5
		color: timer.isTimerWorking ? "red" : "brown"
		anchors.top: parent.top
		anchors.topMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.right: parent.right
		anchors.rightMargin: 0
	}

	Item {
		id: timeLeftItem
		height: 20
		anchors.top: topLine.bottom
		anchors.topMargin: 5
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.right: parent.right
		anchors.rightMargin: 0
		visible: timer.isSessionOngoing

		Text {
			id: timeStartTxt
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 10
			font.pixelSize: 14
			font.family: "Verdana"
			text: "Start time: " + timer.timeStartStr
		}

		Text {
			id: timeLeftTxt
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 10
			font.pixelSize: 14
			font.family: "Verdana"
			text: timer.timeWritingCodeStr + " left"
		}
		Connections {
			target: timer
			onTimeWritingCodeChanged: timeLeftTxt.text = timer.timeWritingCodeStr  + " left"
			onTimeStartChanged: timeStartTxt.text = "Start time: " + timer.timeStartStr
		}
	}

	QtC22.SwipeView {
		id: swipeView
		width: parent.width
		anchors.top: timeLeftItem.bottom
		anchors.topMargin: 0
		currentIndex: 1
		SettingsLayer {}
		DialLayer{}
		StatisticsLayer{}
	}

	NavigationLayer {
		id: swipeItem
		width: parent.width
		anchors.bottom: bottomLine.top
		anchors.bottomMargin: 15
	}

	Rectangle {
		id: bottomLine
		height: 3
		color: topLine.color
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.right: parent.right
		anchors.rightMargin: 0

	}
}
