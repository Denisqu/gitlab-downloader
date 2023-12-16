// import Material 0.3
import FluentUI 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

//window containing the application
ApplicationWindow {
    title: "Buttons"
    visible: true
    height: 600
    width: 900
    minimumHeight: 600
    minimumWidth: 900

    ColumnLayout {
        anchors.fill: parent


        RowLayout {
            Layout.fillHeight: false
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            FluText {
                Layout.alignment: Qt.AlignLeft
                text: "Current status: No downloads needed"
            }
            FluText {
                Layout.alignment: Qt.AlignLeft
                text: "Refreshed last time: 10.12.2023 11:01"
            }
            FluButton {
                text: "Settings"
                Layout.alignment: Qt.AlignRight
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.alignment: Qt.AlignCenter
            model: TestModel {}
            delegate: Text {
                text: name + ": " + number
            }
        }

        RowLayout {
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignRight
            FluButton {
                text: "Refresh all"
                Layout.alignment: Qt.AlignRight
            }
            FluButton {
                text: "Download new artifacts"
                Layout.alignment: Qt.AlignRight
            }
        }
    }
}
