import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

Item {
    id: item1
    property int buttonHeight: 30
    property int buttonWidth: 128
    property int textPointSize: 10
    property date currentDate: new Date()
    property bool isModelViewLoaded: true
    property string defautElement: "qrc:/qml/ModelView.qml"

    signal showResult()

    onShowResult: {
        loader.source = defautElement;
        isModelViewLoaded = true;
    }
    Component.onCompleted: {
        showResult();
    }

    //=============================
    // Label Info
    Text {
        id: txtLabel
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 16
        text: "Statistics:"
    }
    //=============================
    // Buttons panel
    ListView {
        id: menuList
        width: 130
        height: 220
        anchors.top: txtLabel.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        spacing: 5
        interactive: false
        highlightFollowsCurrentItem: true
        model: StatisticsMenuItem {}
        delegate: StatisticsMenuAnimation {}
    }
    //=============================
    // Elements view
    Item {
        id: componentView
        anchors.top: txtLabel.bottom
        anchors.topMargin: 5
        anchors.left: menuList.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: window.height - 125
    }
    Loader{
        id: loader
        anchors.fill: componentView
    }
    //=============================
    MessageDialog {
        id: dialogRemoveAll
        title: qsTr("Remove All Records")
        text: qsTr("Do you confirm to remove ALL records from current table?")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            dataBase.removeAllRecords();
            databaseModel.updateModel();
        }
    }

} // item
