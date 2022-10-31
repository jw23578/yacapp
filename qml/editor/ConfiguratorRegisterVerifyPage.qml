import QtQuick 2.15
import "../items"
import ".."

Rectangle
{
    id: registerVerifyPage
    anchors.fill: parent
    function show(email)
    {
        loginEMail.text = email
        registerVerifyPage.visible = true
    }

    Column
    {
        id: theColumn
        anchors.centerIn: parent
        spacing: 1
        width: parent.width * 3 / 4
        YACLineEditWithHeader
        {
            headerText: qsTr("LoginEMail")
            width: parent.width
            id: loginEMail
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("Password")
            width: parent.width
            id: password
            echoMode: TextInput.Password
        }
        YACButton
        {
            text: qsTr("Register")
            width: parent.width
            onClicked: configurator.yacserverRegister(loginEMail.displayText, password.text,
                                                      function(message)
                                                      {
                                                          yacApp.goodMessage(message, null, null)
                                                      },
                                                      function(message)
                                                      {
                                                          yacApp.badMessage(message, null, null)
                                                      }
                                                      )
        }

        YACLineEditWithHeader
        {
            headerText: qsTr("Verify-Code")
            width: parent.width
            id: verifyToken
        }
        YACButton
        {
            text: qsTr("Verify")
            width: parent.width
            onClicked: configurator.yacserverVerify(loginEMail.displayText, verifyToken.displayText)
        }
        YACButton
        {
            text: qsTr("Abbrechen")
            onClicked: registerVerifyPage.visible = false
            width: parent.width
        }
    }
    Connections
    {
        target: configurator
        function onVerifySuccessful()
        {
            yacApp.goodMessage(qsTr("Verifyíing Successful, you are now logged in an can deploy your App."), null, null)
            registerVerifyPage.visible = false
        }
        function onVerifyNotSuccessful(message)
        {
            yacApp.badMessage(message, null, null)
        }
    }
}
