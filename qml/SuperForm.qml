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
    property bool currentSuperForm: stackView.currentItem == theSuperForm
    color: config.background.color

    Loader
    {
        anchors.fill: parent
        sourceComponent: config.content.type == "webview" ? webviewComponent :
                                                            config.content.type == "grid" ? gridComponent :
                                                                                            config.content.type == "column" ? columnComponent :
                                                                                                                              config.content.type == "row" ? rowComponent : null
    }

    Component
    {
        id: webviewComponent
        Rectangle
        {
            color: "yellow"
            id: webviewItem
            visible: !theMenue.menueOpen && !Constants.topLevelFormActive && !Constants.profileOpen  && !Constants.superMenueOpen && !Constants.superMenueClicked &&  (!config.content.loginNeeded || yacApp.loginToken != "")
            enabled: visible
            anchors.fill: parent
            WebView
            {
                id: theWebview
                anchors.fill: parent
                visible: true // !loading && theSuperForm.currentSuperForm && !Constants.topLevelFormActive
                url: config.content.url + (config.content.loginNeeded ? ThirdPartyLogin.loginToken : "")
                onUrlChanged: Helper.jsLog("url: " + url)
                onVisibleChanged:
                {
                    Helper.jsLog("Webview Visible: " + visible)
                    Helper.jsLog("Webview Constants.topLevelFormActive: " + Constants.topLevelFormActive)
                }
                onStateChanged:
                {
                    Helper.jsLog("Webview loading: " + theWebview.loading)
                    Helper.jsLog("Webview State: " + theWebview.state)
                }
            }
            Rectangle
            {
                anchors.fill: parent
                id: webviewLoadingRectangle
                visible: false // theWebview.loading
                YACText
                {
                    anchors.centerIn: parent
                    text: qsTr("One Moment please")
                }
            }
        }
    }


    Component
    {
        id: gridComponent

        Flickable
        {
            id: theGridFlickable
            anchors.fill: parent
            contentHeight: theContentGrid.height
            contentWidth: theContentGrid.width
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
    }

    Component
    {
        id: columnComponent

        Flickable
        {
            clip: true
            id: theColumnFlickable
            anchors.fill: parent
            contentHeight: theContentColumn.height
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
    }

    Component
    {
        id: rowComponent

        Flickable
        {
            clip: true
            id: theRowFlickable
            anchors.fill: parent
            contentWidth: theContentRow.width
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
    }

    Loader
    {
        active: false
        id: loginPageLoader
        anchors.fill: parent
        focus: true
        source: config.content.loginNeeded && ThirdPartyLogin.loginToken == "" ? "LoginPage.qml" : ""
    }
    Component.onCompleted: loginPageLoader.active = true
}
