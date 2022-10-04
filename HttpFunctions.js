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
