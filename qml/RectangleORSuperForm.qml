import QtQuick 2.15
import "qrc:/qml/items"

Item
{
    id: theItem
    property string filename: ""
    property alias color: theRectangle.color
    property alias text: theText.text
    property var stackView: null
    property var theMenue: null
    YACRectangle
    {
        id: theRectangle
        visible: theItem.filename == ""
        anchors.fill: parent
        shadowEnabled: false
        YACText
        {
            id: theText
            anchors.centerIn: parent
        }
    }
    Loader
    {
        active: theItem.filename != ""
        asynchronous: true
        anchors.fill: parent
        source: "SuperForm.qml"
        visible: active
        onLoaded:
        {
            item.stackView = theItem.stackView
            item.config = yacApp.getConfig(theItem.filename)
            item.theMenue = theItem.theMenue
        }
    }
}
