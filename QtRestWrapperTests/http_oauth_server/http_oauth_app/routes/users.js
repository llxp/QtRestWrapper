var express = require('express');
var router = express.Router();
const url = require('url'); // built-in utility

const upgradeToken = require('./upgradeToken');
const verifyToken = require('./verifyToken');

const renderPage = function(res) {
	res.send('<html><body><h1>protected content</h1></body></html>');
};

const renderErrorPage = function(res) {
	res.send('<html><body><h1>Error upgrading the token!</h1></body></html>');
};

const redirectToSsoLogin = function(res, fullUrl) {
	res.redirect("https://localhost:3001/simplesso/login?serviceURL=" + fullUrl);
};

/* GET users listing. */
router.get('/', function(req, res, next) {
	const fullUrl = req.protocol + '://' + req.get('host') + req.originalUrl;
	if(typeof(req.session.authToken) !== typeof(undefined)) {
		// verify the token
		verifyToken(req.protocol + '://' + req.get('host'), req.session.authToken, () => {
			renderPage(res);
		}, () => {
			delete req.session.authToken;
			redirectToSsoLogin(res, fullUrl);
		});
	} else if(typeof(req.query.ssoToken) !== typeof(undefined)) {
		// upgrade the token
		upgradeToken(req.protocol + '://' + req.get('host'), req.query.ssoToken, (bearerToken) => {
			req.session.authToken = bearerToken;
			//renderPage(res);
			const parsedUrl = url.parse(fullUrl);
			console.log('redirect to users page without the query: '  + 'https://localhost:3000/protectedContent');
			//delete req.query;
			//res.redirect('https://localhost:3000/users');
			//res.redirect(302, 'https://localhost:3000/protectedContent');
			//req.query = {};
			//delete req.query;
			//delete req;
			res.redirect(req.protocol + '://' + parsedUrl.host + parsedUrl.pathname);
		}, () => {
			console.log("failed to upgrade the token");
			renderErrorPage(res);
		});
	} else {
		console.log('redirect to sso server login');
		redirectToSsoLogin(res, fullUrl);
	}
});

module.exports = router;
