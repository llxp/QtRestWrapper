var express = require('express');
var router = express.Router();
var https = require('https');

const verifyToken = require('./verifyToken');

const renderPage = function(res) {
	return res.status(200).json({ status: 'success'});
};

/* GET users listing. */
router.get('/', function(req, res, next) {
	return renderPage(res);
});

module.exports = router;