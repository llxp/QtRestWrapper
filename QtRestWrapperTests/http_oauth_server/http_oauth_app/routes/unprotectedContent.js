var express = require('express');
var router = express.Router();
var https = require('https');

const verifyToken = require('./verifyToken');

const renderPage = function(res) {
	res.send('<html><body><h1>unprotected content</h1></body></html>');
};

/* GET users listing. */
router.get('/', function(req, res, next) {
	renderPage(res);
});

module.exports = router;