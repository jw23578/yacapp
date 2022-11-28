import QtQuick 2.15
import "../items"

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
            id: registerColumn
            visible: true
            YACLineEditWithHeader
            {
                id: login
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: password
                width: parent.width
                echoMode: TextInput.Password
                headerText: qsTr("Password")
                color: Helper.passwordOk(text) ? Constants.goodColor : Constants.badColor
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Register")
                onClicked:
                {
                    if (!Helper.emailIsValid(login.displayText))
                    {
                        yacApp.badMessage(qsTr("Please enter a valid E-Mail as Login."), login, null)
                        return
                    }
                    if (!Helper.passwordOk(password.text))
                    {
                        yacApp.badMessage(qsTr("Please enter a better Password."), password, null)
                        return
                    }
                    yacApp.appUserRegister(login.displayText,
                                          password.text,
                                          function() {},
                                          function() {})
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Verify")
                onClicked:
                {
                    if (login.displayText != "")
                    {
                        verifyLogin.text = login.displayText
                    }

                    registerColumn.visible = false
                    verifyColumn.visible = true
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Back")
            }
        }
        YACPageColumn
        {
            id: verifyColumn
            visible: false
            YACLineEditWithHeader
            {
                id: verifyLogin
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: verifyToken
                width: parent.width
                headerText: qsTr("Code")
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Verify")
                onClicked:
                {
                    if (!Helper.emailIsValid(verifyLogin.displayText))
                    {
                        yacApp.badMessage(qsTr("Please enter a valid E-Mail as Login."), verifyLogin, null)
                        return
                    }
                    if (verifyToken.displayText == "")
                    {
                        yacApp.badMessage(qsTr("Please enter the code you received by E-Mail."), verifyToken, null)
                        return
                    }

                    yacApp.appUserVerify(verifyLogin.displayText,
                                          verifyToken.displayText,
                                          function() {},
                                          function() {})
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Back")
                onClicked:
                {
                    registerColumn.visible = true
                    verifyColumn.visible = false
                }
            }
        }
    }
}
