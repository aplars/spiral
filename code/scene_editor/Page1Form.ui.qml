import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import sa 1.0

Item {
    property alias textField1: textField1
    property alias button1: button1

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        TextField {
            id: textField1
            placeholderText: qsTr("Text Field")
        }

        Button {
            id: button1
            text: qsTr("Press Me")
        }

        EngineView {
        }
    }
}
