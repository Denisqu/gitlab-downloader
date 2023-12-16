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

    FluText {
        Layout.topMargin: 20
        text: "flu text..."
    }

    FluArea {
        Layout.fillWidth: true
        height: 68
        paddings: 10
        Layout.topMargin: 20

        FluTextButton {
            disabled: text_button_switch.checked
            text: "Text Button"
            contentDescription: "content description"
            onClicked: {
                showInfo("Text Button");
            }

            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }

        }

        FluToggleSwitch {
            id: text_button_switch

            text: "Disabled"

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

        }

        FluTextBox {
            id: textbox

            text: "TextBox!!"
        }

        FluIcon {
            id: icon
        }

        FluCheckBox {
            text: "checkbox"
        }

    }

}
