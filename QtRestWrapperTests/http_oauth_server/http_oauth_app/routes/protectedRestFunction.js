var express = require('express');
var router = express.Router();
var https = require('https');
const fromAuthHeaderAsBearerToken = require('./fromAuthHeaderAsBearerToken');
const appTokenFromRequest = fromAuthHeaderAsBearerToken();
const verifyToken = require('./verifyToken');

const renderPage = function(res) {
	return res.status(200).json({ status: 'success'});
};

/* GET users listing. */
router.get('/', function(req, res, next) {
	const token = appTokenFromRequest(req);
	const fullUrl = req.protocol + '://' + req.get('host') + req.originalUrl;
	if(typeof(token) !== typeof(undefined)) {
		// verify the token
		verifyToken(req.protocol + '://' + req.get('host'), req.session.authToken, () => {
			renderPage(res);
		}, () => {
			return res.status(403).json({status: 'unauthorized'});
		});
	} else if(typeof(req.session.authToken) !== typeof(undefined)) {
		// verify the token
		verifyToken(req.protocol + '://' + req.get('host'), req.session.authToken, () => {
			renderPage(res);
		}, () => {
			delete req.session.authToken;
			res.redirect(302, '/users');
		});
	} else {
		return res.status(403).json({status: 'unauthorized'});
	}
});

module.exports = router;