import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.menueconfig 1.0
import QtWebView 1.15

import "items"

Rectangle
{
    id: theSuperForm
    property ParsedConfig config: null
    property MenueConfig menue: yacApp.getMenueConfig("")
    property var stackView: null
    color: config.background.color
    WebView
    {
        anchors.fill: parent
        visible: config.content.type == "webview"
        url: config.content.url
    }

    Flickable
    {
        id: theColumnFlickable
        anchors.fill: parent
        contentHeight: theContentColumn.height
        visible: config.content.type == "column"
        Column
        {
            id: theContentColumn
            width: parent.width
            spacing: 1
            Repeater
            {
                model: config.content.itemCount
                delegate: Loader {
                    active: true;
                    asynchronous: true
                    width: theContentColumn.width
                    height: theSuperForm.height * (config.content.type == "column" ? config.content.items[index].height : config.content.items[index].width)
                    source: "SuperForm.qml"
                    visible: true
                    onLoaded: {
                            item.stackView = theSuperForm.stackView
                            item.config = yacApp.getConfig(config.content.items[index].filename)
                        }
                }
            }
        }
    }

    Flickable
    {
        id: theRowFlickable
        anchors.fill: parent
        contentWidth: theContentRow.width
        visible: config.content.type == "row"
        Row
        {
            id: theContentRow
            height: parent.height
            spacing: 1
            Repeater
            {
                model: config.content.itemCount
                delegate: Loader {
                    active: true;
                    asynchronous: true
                    height: theContentRow.height
                    width: theSuperForm.width * config.content.items[index].size
                    source: "SuperForm.qml"
                    visible: true
                    onLoaded: {
                            item.stackView = theSuperForm.stackView
                            item.config = yacApp.getConfig(config.content.items[index].filename)
                        }
                }
            }
        }
    }

    Column
    {
        id: defaultMenue
        visible: menue.type === "" || menue.type === "default"
        anchors.centerIn: parent
        spacing: 1
        width: parent.width * 3 / 4

        Repeater
        {
            model: menue.itemCount
            YACButton
            {
                width: parent.width
                text: menue.items[index].caption
                onClicked:
                {
                    if (menue.items[index].specialMeaning == "back")
                    {
                        stackView.pop()
                        return
                    }
                    if (menue.items[index].specialMeaning == "home")
                    {
                        stackView.pop(null)
                        return
                    }
                    if (menue.items[index].specialMeaning == "clear")
                    {
                        stackView.pop(null)
                    }

                    stackView.push("SuperForm.qml", {
                                       "config": yacApp.getConfig(menue.items[index].targetFile),
                                       "stackView": stackView
                                   }
                                   )
                }
            }
        }
    }
    Component.onCompleted:
    {
        if (config == null)
        {
            return
        }

        menue = yacApp.getMenueConfig(config.menueFilename)
    }
}
