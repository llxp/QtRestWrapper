var express = require('express');
var router = express.Router();

module.exports = function(routes) {
	/* GET home page. */
	router.get('/', function(req, res, next) {
		/*app._router.stack.forEach(function(r){
			if (r.path && r.path){
				console.log("route: " + r.path)
			}
		});*/
		res.render('index', { title: 'Express', routes: routes });
	});
	return router;
};