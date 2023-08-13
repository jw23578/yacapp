import QtQuick 2.15

Rectangle
{
    id: theMultiSelectItem
    property alias model: theListView.model
    property var innerDelegateComponent: null
    property var previousSelected: new Set()
    property var newSelected: new Set()
    property var newDeselected: new Set()
    Component
    {
        id: defaultInnerDelegate
        Rectangle
        {
            property var dataObject: null
            anchors.fill: parent
            color: "red"
        }
    }

    ListView
    {
        id: theListView
        anchors.fill: parent
        clip: true
        spacing: 1
        delegate: Rectangle
        {
            id: theDelegate
            width: theListView.width
            height: 100
            property bool selected: theMultiSelectItem.previousSelected.has(dataObject.id)
            Item
            {
                id: theSelectItem
                height: parent.height
                width: height
                Image
                {
                    anchors.centerIn: parent
                    width: parent.width * 0.7
                    height: width
                    fillMode: Image.PreserveAspectFit
                    source: theDelegate.selected ? "qrc:/images/images/checkbox-active-svgrepo-com.svg" : "qrc:/images/images/checkbox-passive-svgrepo-com.svg"
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        theDelegate.selected = !theDelegate.selected
                        if (theDelegate.selected)
                        {
                            if (theMultiSelectItem.previousSelected.has(dataObject.id))
                            {
                                theMultiSelectItem.newDeselected.delete(dataObject.id)
                            }
                            else
                            {
                                theMultiSelectItem.newSelected.add(dataObject.id)
                            }
                        }
                        else
                        {
                            if (theMultiSelectItem.previousSelected.has(dataObject.id))
                            {
                                theMultiSelectItem.newDeselected.add(dataObject.id)
                            }
                            else
                            {
                                theMultiSelectItem.newSelected.delete(dataObject.id)
                            }
                        }
                        Helper.jsLog("previous: " + Array.from(theMultiSelectItem.previousSelected).join(' '))
                        Helper.jsLog("newSelected: " + Array.from(theMultiSelectItem.newSelected).join(' '))
                        Helper.jsLog("newDeselected: " + Array.from(theMultiSelectItem.newDeselected).join(' '))
                    }
                }
            }

            Loader
            {
                id: innerDelegateLoader
                anchors.left: theSelectItem.right
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                sourceComponent: innerDelegateComponent == null ? defaultInnerDelegate : innerDelegateComponent
                onLoaded: item.dataObject = dataObject
            }
        }
    }
}
