import QtQuick 2.0
import QtWebView 1.15
import com.yacapp.contentitem 1.0

Item
{
    property int formHeight: 0
    property int formWidth: 0
    property string contentType: ""
    property bool columnType: contentType == "column"
    property bool rowType: contentType == "row"
    property ContentItem itemConfig: null
    width: columnType ? parent.width : formWidth * itemConfig.width
    height: rowType ? parent.height : formHeight * itemConfig.height
    Image
    {
        visible: itemConfig.type == "image"
        anchors.fill: parent
        source: visible ? itemConfig.url : ""
    }
    WebView
    {
        visible: itemConfig.type == "webview"
        anchors.fill: parent
        url: visible ? itemConfig.url : ""
    }

    Loader {
        active: itemConfig.type == "file"
        asynchronous: true
        anchors.fill: parent
        source: "SuperForm.qml"
        visible: active
        onLoaded: {
            item.stackView = theSuperForm.stackView
            item.config = yacApp.getConfig(itemConfig.filename)
        }
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            console.log("clicked")
        }
    }
}
