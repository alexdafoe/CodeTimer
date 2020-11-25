import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    Connections {
        target: timer
        onTimerTrigger: if(window.visible === true) timerTxt.text = timer.timerStr
    }

    //=============================
    // Time now Label
    //=============================
    Item {
        id:dialLayer
        height: 120
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Rectangle {
            id: timeNowRect
            height: 110
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10

            border.width: 1
            border.color: "#747474"
            color: "#ffffff"
            Text {
                id: labelTxt
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                color: "gray"
                font.family: "Verdana"
                font.pixelSize: 20
                text: "TIME NOW:"
            }

            Text {
                id: timerTxt
                anchors.top: labelTxt.bottom
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "black"
                font.family: "Arial"
                font.letterSpacing: 5
                font.pixelSize: 56
                text: timer.timerStr
            }
        }
    }
    //=============================
    // Buttons
    //=============================
    Item {
        id: itemButtons
        height: 100
        anchors.top: dialLayer.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        //=============================
        // button Start / Pause
        Item {
            id: itemBtnStart
            width: parent.width / 2
            height: parent.height

            Rectangle {
                id: btnRectStart
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: 180
                height: 60
                radius: 8
                color: "#353637"

                Text {
                    id: btnTextStart
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    color: "white"
                    text: timer.isTimerWorking ?
                              qsTr("Pause") :
                              timer.isSessionOngoing ?
                              qsTr("Resume") :
                              qsTr("Start")
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { btnRectStart.color = "#272829" }
                    onExited: { btnRectStart.color = "#353637" }
                    onClicked: {
                        if(timer.isTimerWorking){
                            timer.Pause();
                        } else {
                            timer.Start();
                        }
                    }
                }
            } // end button rectangle Start/Pause
        } // end of (item) field button Start/Pause
        //=============================
        // button Stop
        Item {
            id: itemBtnStop
            x: itemBtnStart.width
            width: parent.width / 2
            height: parent.height

            Rectangle {
                id: btnRectStop
                x: itemBtnStart.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: 180
                height: 60
                radius: 8
                color: "#353637"

                Text {
                    id: btnTextStop
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    color: "white"
                    text: qsTr("Stop")
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { btnRectStop.color = "#272829" }
                    onExited: { btnRectStop.color = "#353637" }
                    onClicked: {
                        timer.stop();
                    }
                }
            } // end button rectangle Stop
        } // end of (item) field button Stop
    } // end of field buttons area
}
