import QtQuick 2.15
import QtWebView 1.15
import com.yacapp.contentitem 1.0
import "qrc:/MenueFunctions.js" as MenueFunctions
import "qrc:/qml/items"


Item
{
    property var stackView: null
    property var theMenue: null
    property int formHeight: 0
    property int formWidth: 0
    property string contentType: ""
    property bool gridType: contentType == "grid"
    property bool columnType: contentType == "column"
    property bool rowType: contentType == "row"
    property ContentItem itemConfig: null
    width: columnType && !gridType ? parent.width : formWidth * itemConfig.widthPerThousand / 1000.0
    height: rowType && !gridType ? parent.height : formHeight * itemConfig.heightPerThousand / 1000.0

    Component
    {
        id: tileComponent
        YACRectangle
        {
            anchors.fill: parent
            color: itemConfig.color
            YACText
            {
                anchors.centerIn: parent
                text: itemConfig.text
            }
        }
    }

    Component
    {
        id: imageComponent
        Image
        {
            anchors.fill: parent
            source: itemConfig.url
            fillMode: Image.Stretch
            cache: true
        }
    }
    Component
    {
        id: webviewComponent
        WebView
        {
            anchors.fill: parent
            url: itemConfig.url
        }
    }

    Loader
    {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: itemConfig.type == "image" ? imageComponent
                                                    : itemConfig.type == "webview" ? webviewComponent
                                                                                   : itemConfig.type == "tile" ? tileComponent : null
    }


    Loader
    {
        active: itemConfig.type == "file"
        asynchronous: true
        anchors.fill: parent
        source: "SuperForm.qml"
        visible: active
        onLoaded:
        {
            item.stackView = theSuperForm.stackView
            item.config = yacApp.getConfig(itemConfig.filename)
            item.theMenue = theSuperForm.theMenue
        }
    }
    Loader
    {
        id: mouseAreaLoader
        anchors.fill: parent
        sourceComponent: itemConfig.target != "" ? mouseAreaComponent : null
    }
    Component
    {
        id: mouseAreaComponent
        MouseArea
        {
            anchors.fill: parent
            onClicked: MenueFunctions.openTarget(yacApp, stackView, itemConfig.target, theMenue)
        }
    }

}
