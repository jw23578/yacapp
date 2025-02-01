import QtQuick 2.15

GridView {
    clip: true
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: Constants.defaultMargin
    reuseItems: true
}
