import QtQuick 2.0
import QtQuick.Controls 2.2
Item {
    function setDefaultDate() {
        boxDayFrom.currentIndex = Qt.formatDateTime(currentDate, "dd");
        boxMonthFrom.currentIndex = Qt.formatDateTime(currentDate, "MM");
        boxYearFrom.text = Qt.formatDateTime(currentDate, "yyyy");
        boxDayTo.currentIndex = Qt.formatDateTime(currentDate, "dd");
        boxMonthTo.currentIndex = Qt.formatDateTime(currentDate, "MM");
        boxYearTo.text = Qt.formatDateTime(currentDate, "yyyy");
    }

    Component.onCompleted: {
        setDefaultDate();
    }

    //=============================
    // Search Period
    //=============================
    Text {
        id: searchPeriodTxt
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pointSize: textPointSize
        text: "Dates span search:"
    }

    Grid {
        id: gridSearchPeriod
        anchors.top: searchPeriodTxt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        spacing: 10
        rows: 3
        columns: 4
        Text { // top left empty field
            text: " "
        }
        //=============================
        Text {
            id: dayPeriodTxt
            font.pointSize: textPointSize
            text: "Day"
        }
        Text {
            id: monthPeriodTxt
            font.pointSize: textPointSize
            text: "Month"
        }
        Text {
            id: yearPeriodTxt
            font.pointSize: textPointSize
            text: "Year"
        }
        // ...From
        Text {
            id: fromPeriodTxt
            font.pointSize: textPointSize
            text: "From:"
        }
        ComboBox {
            id: boxDayFrom
            width: 80
            height: 30
            model: 32
            validator: IntValidator{ bottom: 1; top: 31;}
            onCurrentIndexChanged: {
                if(boxDayFrom.currentIndex < 1) boxDayFrom.currentIndex = 1;
            }


        }
        ComboBox {
            id: boxMonthFrom
            width: 80
            height: 30
            model: 13
            validator: IntValidator{ bottom: 1; top: 12;}
            onCurrentIndexChanged: {
                if(boxMonthFrom.currentIndex < 1) boxMonthFrom.currentIndex = 1;
            }

        }
        Rectangle {
            id: rectPeriodYearFrom
            width: 80
            height: 30
            color: "white"

            TextInput {
                id: boxYearFrom
                width: parent.width
                height: parent.height
                color: "black"
                font.pixelSize: 12
                maximumLength: 4
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                verticalAlignment: TextInput.AlignVCenter
                validator: IntValidator{ bottom: 2000; top: 2050;}
            }
        }
        //=============================
        // ...To
        Text {
            id: toPeriodTxt
            font.pointSize: textPointSize
            text: "To:"
        }
        ComboBox {
            id: boxDayTo
            width: 80
            height: 30
            model: 32
            validator: IntValidator{ bottom: 1; top: 31;}
            onCurrentIndexChanged: {
                if(boxDayTo.currentIndex < 1) boxDayTo.currentIndex = 1;
            }

        }
        ComboBox {
            id: boxMonthTo
            width: 80
            height: 30
            model: 13
            validator: IntValidator{ bottom: 1; top: 12;}
            onCurrentIndexChanged: {
                if(boxMonthTo.currentIndex < 1) boxMonthTo.currentIndex = 1;
            }

        }
        Rectangle {
            id: rectPeriodYearTo
            width: 80
            height: 30
            color: "white"

            TextInput {
                id: boxYearTo
                width: parent.width
                height: parent.height
                color: "black"
                font.pixelSize: 12
                maximumLength: 4
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                verticalAlignment: TextInput.AlignVCenter
                validator: IntValidator{ bottom: 2000; top: 2050;}
            }
        }
    } // end grid

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
            var dateFrom = new Date(boxYearFrom.getText(0, 5),
                                                boxMonthFrom.currentIndex -1,
                                                boxDayFrom.currentIndex)
            var dateTo = new Date(boxYearTo.getText(0, 5),
                                            boxMonthTo.currentIndex -1,
                                            boxDayTo.currentIndex)
            if(boxMonthFrom.currentIndex === 0 || boxDayFrom.currentIndex === 0){
                dateFrom.setDate(1);
                dateFrom.setMonth(0);
                dateFrom.setFullYear(2000);
                databaseModel.searchPeriod(dateFrom, dateTo);
            }
            if(boxMonthTo.currentIndex === 0 || boxDayTo.currentIndex === 0){
                databaseModel.searchPeriod(dateFrom, currentDate);
            } else {
                databaseModel.searchPeriod(dateFrom, dateTo);
            }
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
        onClicked: setDefaultDate();
    } // end button Clear
}
