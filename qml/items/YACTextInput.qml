import QtQuick 2.15
import "../../JSConstants.js" as JSConstants

TextInput
{
    property alias emptyText: theEmptyText
    property bool useInConfigurator: false
    selectByMouse: Constants.isDesktop
    focus: true
    font.pixelSize: useInConfigurator ? Constants.configuratorDefaultFontPixelSize : Constants.defaultFontPixelSize
    font.family: JSConstants.urbanistMedium
    color: Constants.inputTextColor
    Text
    {
        z: -1
        id: theEmptyText
        opacity: parent.displayText === "" ? 1 : 0.7
        Behavior on opacity {
            NumberAnimation {
                duration: Constants.slowAnimationDuration
            }
        }

        Behavior on anchors.horizontalCenterOffset {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }
        anchors.horizontalCenterOffset: parent.displayText === "" ? 0 : parent.width / 2 - contentWidth / 2
        Behavior on anchors.verticalCenterOffset {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }
        anchors.verticalCenterOffset: parent.displayText === "" ? 0 : parent.height / 2 - contentHeight / 2
        anchors.centerIn: parent
        font.family: parent.font.family
        font.pixelSize: parent.font.pixelSize *  (parent.displayText === "" ? 0.8 : 0.6)
        color: Qt.darker(parent.color)
    }
}
