var express = require('express');
var router = express.Router();

const https = require('https');

const client_secret = "l1Q7zkOL59cRqWBkQ12ZiGVW2DBL";

/* GET users listing. */
router.get('/', function(req, res, next) {
	console.log("query: " + req.query);
	console.log(req.query);
	const fullUrl = req.protocol + '://' + req.get('host') + req.originalUrl;
	console.log(req.session.authToken);
	if(typeof(req.session.authToken) == typeof(undefined) && typeof(req.query.ssoToken) == typeof(undefined)) {
		console.log("redirecting to login page");
		res.redirect("https://localhost:3001/simplesso/login?serviceURL=" + fullUrl);
	} else if(typeof(req.session.authToken) != typeof(undefined)) {
		res.send('<html><body><h1>protected content</h1></body></html>');
	} else if(typeof(req.query.ssoToken) != typeof(undefined) && req.query.ssoToken != null) {
		req.session.ssoToken = req.query.ssoToken;
		//res.send('protected content');
		// verify the token
		console.log("verify the token");
		process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;
		var options = {        
			hostname: 'localhost',
			port: 3001,
			path: '/simplesso/upgradetoken?ssoToken=' + req.session.ssoToken + '&client_secret='+ client_secret,
			method: 'GET',
			/*headers:{
				Authorization: ' Bearer ' + client_secret
			}*/
		};
		https.get(options, (resp) => {
			console.log("token verification started...");
			console.log(resp.statusCode);
			if(resp.statusCode == 200) {
				var data = "";
				resp.on('data', (chunk) => {
					data += chunk;
				});
				resp.on('end', () => {
					req.session.authToken = data.token;
					res.send('<html><body><h1>protected content</h1></body></html>');
				});
			} else {
				res.redirect("https://localhost:3001/simplesso/login?serviceURL=" + fullUrl);
			}
		}).on('error', (err) => {
			console.log("Error: " + err.message);
		});
	}
});

module.exports = router;
