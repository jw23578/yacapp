import QtQuick 2.0
import QtQuick.Controls 2.15

Tumbler
{
    id: theTumbler
    width: parent.width
    height: timeWheel.height - minuteText.height
    Rectangle {
        anchors.horizontalCenter: theTumbler.horizontalCenter
        y: theTumbler.height * (theTumbler.visibleItemCount / 2 - 0.5) / theTumbler.visibleItemCount
        width: 40
        height: 1
        color: "#21be2b"
    }

    Rectangle {
        anchors.horizontalCenter: theTumbler.horizontalCenter
        y: theTumbler.height * (theTumbler.visibleItemCount / 2 + 0.5) / theTumbler.visibleItemCount
        width: 40
        height: 1
        color: "#21be2b"
    }
}
