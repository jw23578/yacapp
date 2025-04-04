import QtQuick 2.15

Loader
{
    z: 2
    id: pageLoader
    property string name: ''
    property bool loginNeeded: false
    property var theComponent: null
    property var parentSuperMenue: null
    property var parentCurrentOpenedLoader: null
    signal opened()
    width: 0
    height: parent.height
    x: parent.width / 2
    y: -Constants.appTotalHeight
    opacity: 1 - y / -Constants.appTotalHeight
    visible: y !== -Constants.appTotalHeight
    property bool isOpen: y === 0
    property int aniDuration: Constants.fastAnimationDuration
    Behavior on width
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    Behavior on x
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    Behavior on y
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    function openBasePageLoader()
    {
        if (y === 0)
        {
            return
        }

        if (loginNeeded)
        {
            if (!yacApp.appUserConfig.loggedIn)
            {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Please login first."), null, null);
                return
            }
        }
        if (parentCurrentOpenedLoader != null)
        {
            parentCurrentOpenedLoader.closeBasePageLoader()
        }
        if (parentSuperMenue != null)
        {
            parentSuperMenue.close()
        }
        sourceComponent = theComponent
        x = 0
        width = parent.width
        y = 0
        opened()
        if (status == Loader.Ready)
        {
            item.multiMenueButton.moveIn()
            item.arrowButton.visible = true
            if (item.theFocusObject !== null)
            {
                item.theFocusObject.forceActiveFocus()
            }
        }
        yacApp.minimizeMenue()
    }

    function closeBasePageLoader()
    {
        console.log("BasePageLoader.qml.close " + name)
        console.log("set y to " + -Constants.appTotalHeight)
        x = theItem.width / 2
        width = 0
        y = -Constants.appTotalHeight
        if (status == Loader.Ready)
        {
            item.multiMenueButton.moveOut()
            item.arrowButton.visible = false
        }
    }
    onLoaded: {
        item.multiMenueButton.moveIn()
        if (item.theFocusObject !== null)
        {
            item.theFocusObject.forceActiveFocus()
        }
    }
    onSourceComponentChanged: Constants.superMenueClicked = sourceComponent != null
}
