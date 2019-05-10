const https = require('https');
const config = require('../config');

const upgradeToken = (originUrl, ssoToken, callbackSuccess, callbackFailure) => {
	process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;
	var options = {        
		hostname: 'localhost',
		port: 3001,
		path: '/simplesso/upgradetoken?ssoToken=' + ssoToken + '&client_secret='+ config.client_secret,
		method: 'GET',
		headers: {
			origin: originUrl
		}
	};
	https.get(options, (resp) => {
		console.log("got a response from sso server: " + resp.statusCode);
		if(resp.statusCode == 200) {
			var data = "";
			resp.on('data', (chunk) => {
				data += chunk;
			});
			resp.on('end', () => {
				const obj = JSON.parse(data);
				if(typeof obj !== typeof undefined) {
					callbackSuccess(obj.token);
				} else {
					callbackFailure();
				}
			});
		} else {
			callbackFailure();
		}
	}).on('error', (err) => {
		console.log("Error: " + err.message);
	});
};

module.exports = upgradeToken;