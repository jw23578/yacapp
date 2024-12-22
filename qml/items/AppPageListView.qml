import QtQuick 2.15

ListView {
    clip: true
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: Constants.defaultMargin
    reuseItems: true
    spacing: Constants.defaultSpacing
}
