.pragma library

.import "HttpFunctions.js" as Http

function aidooLoginCallback(yacApp, data)
{
    var jsontext = data.responseText
    if (jsontext.length < 3)
    {
        return;
    }
    var obj = JSON.parse(jsontext)
    ThirdPartyLogin.loginToken = obj["loginToken"]
    yacApp.saveState()
}

function errorCallback(yacApp, data)
{

}

function abortCallback(yacApp, data)
{

}

function login(yacApp, loginType, url, login, password)
{
    if (loginType == "aidoo")
    {
        Http.request(yacApp, url + "&login=" + login + "&pwd=" + password, aidooLoginCallback, errorCallback, abortCallback)
    }
}
