var express = require('express');
var router = express.Router();
var https = require('https');

const verifyToken = require('./verifyToken');

const renderPage = function(res) {
	res.send('<html><body><h1>protected content</h1></body></html>');
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
			res.redirect(302, '/users');
		});
	} else {
		res.redirect(302, '/users');
	}
});

module.exports = router;