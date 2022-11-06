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
        id: slidertileComponent
        Item
        {
            id: slidertileItem
            clip: true
            anchors.fill: parent
            Row
            {
                id: slidertileRow
                height: slidertileItem.height
                Behavior on x
                {
                    NumberAnimation
                    {
                        duration: 200
                    }
                }

                YACRectangle
                {
                    width: slidertileItem.width
                    height: slidertileItem.height
                    color: itemConfig.color
                    YACText
                    {
                        anchors.centerIn: parent
                        text: itemConfig.text
                    }
                }
                YACRectangle
                {
                    width: slidertileItem.width
                    height: slidertileItem.height
                    color: itemConfig.color2
                    YACText
                    {
                        anchors.centerIn: parent
                        text: itemConfig.text2
                    }
                }
            }
            Timer
            {
                running: true
                interval: 3000
                repeat: true
                onTriggered: slidertileRow.x = slidertileRow.x == 0 ? -slidertileItem.width : 0
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
            visible: !theMenue.menueOpen && !Constants.topLevelFormActive
        }
    }

    Loader
    {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: itemConfig.type == "image" ? imageComponent
                                                    : itemConfig.type == "webview" ? webviewComponent
                                                                                   : itemConfig.type == "tile" ? tileComponent :
                                                                                                                 itemConfig.type == "slidertile" ? slidertileComponent : null
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
