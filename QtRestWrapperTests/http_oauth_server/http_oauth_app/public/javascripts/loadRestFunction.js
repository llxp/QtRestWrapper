function createCORSRequest(method, url) {
  var xhr = new XMLHttpRequest();
  if ("withCredentials" in xhr) {

    // Check if the XMLHttpRequest object has a "withCredentials" property.
    // "withCredentials" only exists on XMLHTTPRequest2 objects.
    xhr.open(method, url, true);

  } else if (typeof XDomainRequest != "undefined") {

    // Otherwise, check if XDomainRequest.
    // XDomainRequest only exists in IE, and is IE's way of making CORS requests.
    xhr = new XDomainRequest();
    xhr.open(method, url);

  } else {

    // Otherwise, CORS is not supported by the browser.
    xhr = null;

  }
  return xhr;
}

function loadRestFunction() {
	var xhr = createCORSRequest('GET', url);
	xhr.setRequestHeader('Access-Control-Max-Age', 60*60*5);
	xhr.onreadystatechange = function(xhrEvent) {
		if(xhr.readyState === 4) {
			const responseText = xhr.responseText;
			const parsedResponse = JSON.parse(responseText);
			if(xhr.status === 200) {
				console.log(parsedResponse);
			}
		}
	};
	xhr.send();
}