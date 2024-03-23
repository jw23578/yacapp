import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/ModelFunctions.js" as ModelFunctions
import "../../JSConstants.js" as JSConstants

Column
{
    width: parent.width
    property string initialText: ""
    property alias headerText: header.text
    property alias model: input.model
    property alias currentText: input.currentText
    property alias currentIndex: input.currentIndex
    property alias comboBox: input
    function find(needle)
    {
        return ModelFunctions.find(input.model, needle)
    }

    signal activated(string text)

    Text
    {
        id: header
        width: parent.width
    }
    ComboBox
    {
        id: input
        width: parent.width
        currentIndex: initialText != "" ? find(initialText) : 0
        onActivated: parent.activated(textAt(index))

        background: Rectangle {
            implicitWidth: 120
            implicitHeight: 40
            color: input.popup.visible  ? "white" : input.pressed ? Constants.buttonDownColor : Constants.buttonColor
            border.color: Constants.buttonColor
            radius: height / 4
        }

        popup: Popup {
            y: input.height - 1
            width: input.width
            implicitHeight: contentItem.implicitHeight + 1
            padding: 1

            contentItem: ListView
            {
                clip: true
                id: theListview
                implicitHeight: contentHeight + 1
                model: input.popup.visible ? input.delegateModel : null
                currentIndex: input.highlightedIndex

                highlight: YACRectangle
                {
                    color: Constants.buttonColor
                    radius: input.height / 2
                    radiusTopLeft: input.highlightedIndex == 0
                    radiusTopRight: input.highlightedIndex == 0
                    radiusBottomLeft: input.highlightedIndex == theListview.count - 1
                    radiusBottomRight: input.highlightedIndex == theListview.count - 1
                }

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                border.color: Constants.buttonColor
                radius: input.height / 2
            }
        }

        delegate: ItemDelegate
        {
            id: theDelegate
            width: input.width
            height: input.height
            contentItem: Text {
                text: modelData
                color: Constants.fontColor
                font.pixelSize: Constants.defaultFontPixelSize
                font.family: JSConstants.urbanistMedium
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            background: Item {
                implicitWidth: 100
                implicitHeight: input.height

                Rectangle
                {
                    visible: index != input.count -1
                    width: parent.width
                    height: 1
                    color: Constants.buttonColor
                    anchors.bottom: parent.bottom
                }
            }
            highlighted: input.highlightedIndex === index
        }
    }
}
