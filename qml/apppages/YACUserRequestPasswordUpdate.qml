import QtQuick 2.15
import "../items"
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions

FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            id: requestPasswordUpdateColumn
            visible: true
            YACLineEditWithHeader
            {
                id: login
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Request password update")
                onClicked:
                {
                    if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, login.displayText, login))
                    {
                        return;
                    }
                    yacApp.appUserRequestPasswordUpdate(login.displayText,
                                                        function(message) {
                                                            yacApp.goodMessage(qsTr(message), null, null)
                                                        },
                                                        function(message) {
                                                            yacApp.badMessage(qsTr(message), null, null)
                                                        })
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Back")
            }
        }
    }

}
