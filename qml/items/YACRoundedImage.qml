import QtQuick 2.6
import QtQuick.Effects

Item
{
    id: theItem
    property bool rounded: true
    property bool adapt: true
    property double radius: 0
    property bool circled: true
    property alias source: img.source
    property alias autoTransform: img.autoTransform
    Image
    {
        id: img
        visible: parent.rounded
        anchors.fill: parent
        mipmap: true
        fillMode: Image.PreserveAspectFit
    }
    MultiEffect
    {
        visible: parent.rounded
        source: img
        anchors.fill: img
        maskEnabled: parent.rounded
        maskSource: theMask
    }
    Rectangle
    {
        visible: false
        id: theMask
        layer.enabled: true
        anchors.centerIn: parent
        width: parent.adapt ? img.width : Math.min(img.width, img.height)
        height: parent.adapt ? img.height : img.width
        radius: parent.circled ? Math.min(img.width, img.height) : parent.radius
    }
}

