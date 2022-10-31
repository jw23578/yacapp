import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import QtWebView 1.15
import "items"

Rectangle
{
    id: theSuperForm
    property ParsedConfig config: null
    property var stackView: null
    property var theMenue: null
    color: config.background.color
    Item
    {
        id: webviewItem
        visible: !theMenue.menueOpen && config.content.type == "webview" && (!config.content.loginNeeded || yacApp.loginToken != "")
        enabled: visible
        anchors.fill: parent
        WebView
        {
            id: theWebview
            anchors.fill: parent
            visible: !loading
            url: config.content.url + (config.content.loginNeeded ? yacApp.loginToken : "")
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
        enabled: visible
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
                model: theGridFlickable.visible ? config.content.itemCount : 0
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    theMenue: theSuperForm.theMenue
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
        enabled: visible
        boundsBehavior: Flickable.StopAtBounds
        Column
        {
            id: theContentColumn
            width: parent.width
            spacing: 1
            Repeater
            {
                model: theColumnFlickable.visible ? config.content.itemCount : 0
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    theMenue: theSuperForm.theMenue
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
        enabled: visible
        boundsBehavior: Flickable.StopAtBounds
        Row
        {
            id: theContentRow
            height: parent.height
            spacing: 1
            Repeater
            {
                model: theRowFlickable.visible ? config.content.itemCount : 0
                delegate: ContentDelegate
                {
                    stackView: theSuperForm.stackView
                    theMenue: theSuperForm.theMenue
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
    }
    Component.onCompleted: loginPageLoader.active = true
}
