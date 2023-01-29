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
            input.color: emailInValid.visible ? "red" : "black"
        }
        YACText
        {
            id: emailInValid
            text: qsTr("E-Mail is invalid")
            visible: !Helper.emailIsValid(loginEMail.displayText)
            color: "red"
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
            onClicked:
            {
                if (!Helper.emailIsValid(loginEMail.displayText))
                {
                    yacApp.badMessage(qsTr("Please enter a valid email"), loginEMail, null)
                    return
                }
                if (password.text == "")
                {
                    yacApp.badMessage(qsTr("Please enter a valid password"), password, null)
                    return;
                }

                configurator.yacserverRegister(loginEMail.displayText, password.text,
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
            onClicked:
            {
                if (!Helper.emailIsValid(loginEMail.displayText))
                {
                    yacApp.badMessage(qsTr("Please enter a valid email"), loginEMail, null)
                    return
                }
                if (verifyToken.displayText == "")
                {
                    yacApp.badMessage(qsTr("Please enter a valid Verify-Code"), verifyToken, null)
                    return;
                }

                configurator.yacserverVerify(loginEMail.displayText
                                             , verifyToken.displayText
                                             , function()
                                             {
                                                 yacApp.goodMessage(qsTr("Verifyíing Successful, you are now logged in an can deploy your App."), null, null)
                                                 registerVerifyPage.visible = false
                                             }
                                             , function(message)
                                             {
                                                 yacApp.badMessage(message, null, null)
                                             }
)
            }
        }
        YACButton
        {
            text: qsTr("Abort")
            onClicked: registerVerifyPage.visible = false
            width: parent.width
        }
    }
}
