import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    function show(id, name)
    {
        space_id = id
        theName.text = name
        visible = true
    }

    property string space_id: ""
    YACPageColumn
    {
        centerVertical: false
        YACText
        {
            id: theName
        }
        YACLineEditWithHeader
        {
            id: theAccesscode
            headerText: qsTr("Accesscode")
        }
        YACButton
        {
            width: parent.width
            text: qsTr("Request by Code")
            onClicked:
            {
                if (theAccesscode.displayText == "")
                {
                    yacApp.badMessage(qsTr("Please enter Accesscode first"), theAccesscode, null)
                    return
                }
                yacApp.notYetImplemented()
            }
        }
        YACButton
        {
            width: parent.width
            text: qsTr("Request by Admin")
            onClicked: yacApp.notYetImplemented()
        }
    }
}
