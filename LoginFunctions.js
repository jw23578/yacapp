.pragma library

function request(yacApp, url, callback, errorCallback, abortCallback)
{
    var xhr = new XMLHttpRequest();
    xhr.timeout = 5000;
    xhr.onload = (function(myxhr) {
        return function() {
            callback(yacApp, myxhr)
        }
    })(xhr);
    xhr.onerror = (function(myxhr) {
        return function() {
            errorCallback(yacApp, myxhr)
        }
    })(xhr);
    xhr.onabort = (function(myxhr) {
        return function() {
            abortCallback(yacApp, myxhr)
        }
    })(xhr);
    xhr.open('GET', url, true);
    xhr.send('');
}

function aidooLoginCallback(yacApp, data)
{
    var jsontext = data.responseText
    if (jsontext.length < 3)
    {
        return;
    }
    var obj = JSON.parse(jsontext)
    console.log(obj["loginToken"])
    yacApp.loginToken = obj["loginToken"]
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
        request(yacApp, url + "&login=" + login + "&pwd=" + password, aidooLoginCallback, errorCallback, abortCallback)
    }
}
