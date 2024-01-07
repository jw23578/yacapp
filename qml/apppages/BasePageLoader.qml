import QtQuick 2.15

Loader
{
    z: 2
    id: pageLoader
    property bool loginNeeded: false
    property var theComponent: null
    property var parentSuperMenue: null
    property var parentCurrentOpenedLoader: null
    signal opened()
    width: 0
    height: parent.height
    x: parent.width / 2
    y: -height
    property bool isOpen: y == 0
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
    function open()
    {
        if (y == 0)
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
            parentCurrentOpenedLoader.close()
        }
        if (parentSuperMenue != null)
        {
            parentSuperMenue.close()
        }
        sourceComponent = theComponent
        x = 0
        width =  parent.width
        y = 0
        opened()
        if (status == Loader.Ready)
        {
            item.multiMenueButton.moveIn()
            item.backImage.visible = true
        }
        yacApp.minimizeMenue()
    }

    function close()
    {
        x = theItem.width / 2
        width = 0
        y = -height
        if (status == Loader.Ready)
        {
            item.multiMenueButton.moveOut()
            item.backImage.visible = false
        }
    }
    onLoaded: item.multiMenueButton.moveIn()
    onSourceComponentChanged: Constants.superMenueClicked = sourceComponent != null
}
