.pragma library

function checkEMail(Helper, CPPQMLAppAndConfigurator, email, toFocus)
{
    if (!Helper.emailIsValid(email))
    {
        CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid E-Mail."), toFocus, null)
        return false
    }
    return true
}

function checkPassword(Helper, CPPQMLAppAndConfigurator, password, toFocus)
{
    if (!Helper.passwordOk(password))
    {
        CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a better Password."), toFocus, null)
        return false
    }
    return true
}
