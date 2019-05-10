const https = require('https');
const config = require('../config');

const verifyToken = (originUrl, bearerToken, callbackSuccess, callbackFailure) => {
	process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;
	console.log('url: ' + originUrl);
	var options = {        
		hostname: 'localhost',
		port: 3001,
		path: '/simplesso/verifytoken?ssoToken=' + bearerToken + '&client_secret='+ config.client_secret,
		method: 'GET',
		headers: {
			origin: originUrl
		}
	};
	https.get(options, (resp) => {
		console.log("got a response from sso server: " + resp.statusCode);
		if(resp.statusCode == 200) {
			callbackSuccess();
		} else {
			callbackFailure();
		}
	}).on('error', (err) => {
		console.log("Error: " + err.message);
	});
};

module.exports = verifyToken;