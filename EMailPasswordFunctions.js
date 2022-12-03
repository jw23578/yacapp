.pragma library

function checkEMail(Helper, yacApp, email, toFocus)
{
    if (!Helper.emailIsValid(email))
    {
        yacApp.badMessage(qsTr("Please enter a valid E-Mail."), toFocus, null)
        return false
    }
    return true
}

function checkPassword(Helper, yacApp, password, toFocus)
{
    if (!Helper.passwordOk(password))
    {
        yacApp.badMessage(qsTr("Please enter a better Password."), toFocus, null)
        return false
    }
    return true
}
