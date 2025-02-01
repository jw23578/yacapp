import QtQuick 2.15
import QtQuick.Layouts

GridLayout {
    clip: true
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: Constants.defaultMargin
    columnSpacing: Constants.defaultSpacing
    rowSpacing: Constants.defaultSpacing
}
