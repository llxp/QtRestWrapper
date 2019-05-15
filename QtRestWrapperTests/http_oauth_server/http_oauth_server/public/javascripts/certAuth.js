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

function certAuthClicked(event) {
	event.preventDefault();
	//return false;
	var url = new URL(event.target.href);
	if(url.search.length > 0) {
		url.searchParams.set('result', 'return');
	}
	var xhr = createCORSRequest('GET', url);
	xhr.setRequestHeader('Access-Control-Max-Age', 60*60*5);
	xhr.onreadystatechange = function(xhrEvent) {
		if(xhr.readyState === 4) {
			const responseText = xhr.responseText;
			const parsedResponse = JSON.parse(responseText);
			if(xhr.status === 200) {
				// process the response.
				if(parsedResponse.message === 'success') {
					var redirect_uri = new URL(new URL(window.location.href).searchParams.get('redirect_uri'));
					redirect_uri.searchParams.set('ssoToken', parsedResponse.ssoToken);
					window.location.href = redirect_uri;
				}
			} else {
				if(parsedResponse.message === 'redirect_uri is missing') {
					window.location.href = 'https://localhost:3001';
				}
				console.log('There was an error: ', xhr.state, parsedResponse.message);
			}
		}
	};
	xhr.onerror = function(error) {
		console.log(error);
		if(error.type === 'error') {
			var certAuthErrorContainer = document.getElementById('certAuthErrorContainer');
			if(typeof certAuthErrorContainer !== typeof undefined) {
				$('#certAuthErrorContainer').alert();
				if(typeof certAuthErrorContainer !== typeof undefined) {
					$('#loadingSpinnerContainer').innerHTML = '<img src=\'/images/1_9EBHIOzhE1XfMYoKz1JcsQ.gif\'/>';
				}
				certAuthErrorContainer.innerHTML = '\
				<div class=\'alert alert-danger fade show\' data-alert=\'alert\'>\
				The requested client certificate couldn\'t be found on the computer\
				</div>';
				$(".alert").delay(2000).fadeOut(500, function () { $(this).remove(); });
			}
		}
	};
	var loadingSpinnerContainer = document.getElementById('loadingSpinnerContainer');
	if(typeof certAuthErrorContainer !== typeof undefined) {
		certAuthErrorContainer.innerHTML = '<img src=\'/images/1_9EBHIOzhE1XfMYoKz1JcsQ.gif\' width=220, height=145/>';
		console.log(certAuthErrorContainer.innerHTML);
	}
	xhr.send();
	return false;
}