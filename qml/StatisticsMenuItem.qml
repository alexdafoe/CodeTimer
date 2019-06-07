import QtQuick 2.0

ListModel {
    ListElement{
        buttonIndex: 0
        buttonLabel: "Show All Dates"
        element: "qrc:/qml/ModelView.qml"
    }
    ListElement{
        buttonIndex: 1
        buttonLabel: "Date search"
        element: "qrc:/qml/SearchDate.qml"
    }
    ListElement{
        buttonIndex: 2
        buttonLabel: "Note search"
        element: "qrc:/qml/SearchNote.qml"
    }
    ListElement{
        buttonIndex: 3
        buttonLabel: "To Unite Date"
        element: "qrc:/qml/CombineDate.qml"
    }
    /*ListElement{
        buttonLabel: "Remove Row"
        buttonIndex: 4
        element: ""
    }*/
    ListElement{
        buttonLabel: "Remove All Records"
        buttonIndex: 5
        element: ""
    }
}
