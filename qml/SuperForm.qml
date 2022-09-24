import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.menueconfig 1.0
import QtWebView 1.15
import "qrc:/MenueFunctions.js" as MenueFunctions


import "items"

Rectangle
{
    id: theSuperForm
    property ParsedConfig config: null
    property MenueConfig menue: yacApp.getMenueConfig(config.menueFilename)
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
        id: theGridFlickable
        anchors.fill: parent
        contentHeight: theContentGrid.height
        contentWidth: theContentGrid.width
        visible: config.content.type == "grid"
        clip: true
        Grid
        {
            id: theContentGrid
            columns: config.content.columns
            rows: config.content.rows
            spacing: 1
            Repeater
            {
                model: config.content.itemCount
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    contentType: config.content.type
                    itemConfig: config.content.items[index]
                    formHeight: theSuperForm.height
                    formWidth: theSuperForm.width
                }
            }
        }
    }

    Flickable
    {
        clip: true
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
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    contentType: config.content.type
                    itemConfig: config.content.items[index]
                    formHeight: theSuperForm.height
                    formWidth: theSuperForm.width
                }
            }
        }
    }

    Flickable
    {
        clip: true
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
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    contentType: config.content.type
                    itemConfig: config.content.items[index]
                    formHeight: theSuperForm.height
                    formWidth: theSuperForm.width
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
                onClicked: MenueFunctions.openTarget(yacApp, stackView, menue.items[index].target)
            }
        }
    }
}
