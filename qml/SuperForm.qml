import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import QtWebView 1.15
import "items"

Rectangle
{
    id: theSuperForm
    property ParsedConfig config: null
    property var stackView: null
    color: config.background.color
    Item
    {
        id: webviewItem
        visible: config.content.type == "webview"
        anchors.fill: parent
        WebView
        {
            id: theWebview
            anchors.fill: parent
            visible: !loading
            url: config.content.url + (config.content.loginNeeded ? yacApp.loginToken : "")
            Component.onCompleted: console.log("url: " + url)
        }
        Rectangle
        {
            anchors.fill: parent
            id: webviewLoadingRectangle
            visible: theWebview.loading
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("One Moment please")
            }
        }
    }


    Flickable
    {
        id: theGridFlickable
        anchors.fill: parent
        contentHeight: theContentGrid.height
        contentWidth: theContentGrid.width
        visible: config.content.type == "grid"
        clip: true
        boundsBehavior: Flickable.StopAtBounds
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
        boundsBehavior: Flickable.StopAtBounds
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
        boundsBehavior: Flickable.StopAtBounds
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

    Loader
    {
        active: false
        id: loginPageLoader
        anchors.fill: parent
        focus: true
        source: config.content.loginNeeded && yacApp.loginToken == "" ? "LoginPage.qml" : ""
        onStatusChanged: console.log("status: ", status)
    }
    Component.onCompleted: loginPageLoader.active = true
}
