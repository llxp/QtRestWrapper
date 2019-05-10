var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
//var cookieSession = require('cookie-session');
var session = require('express-session');
var MemoryStore = require('memorystore')(session);
var cors = require('express-cors');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');
var protectedContentRouter = require('./routes/protectedContent');
var unprotectedContentRouter = require('./routes/unprotectedContent');
var testRestFunctionRouter = require('./routes/testRestFunction');
var protectedRestFunctionRouter = require('./routes/protectedRestFunction');

var app = express();

const corsOptions = {
  origin: 'https://localhost:3000', //the port my react app is running on.
  credentials: true,
};

app.use(cors(corsOptions));

app.use(
  /*cookieSession({
    name: 'session2',
    keys: ["keyboard cat2"],
    // Cookie Options
    maxAge: 24 * 60 * 60 * 1000 // 24 hours
  })*/
	session({
		store: new MemoryStore({
			checkPeriod: 86400000
		}),
		secret: "keyboard cat3",
    	resave: true,
    	saveUninitialized: true,
    	name: "session3",
    	//secure: false,
    	cookie: { maxAge: 86400000, secure: true }
	})
);

app.use((req, res, next) => {
  console.log(req.session);
  next();
});

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
//app.use('/users', usersRouter);
app.use('/protectedContent', usersRouter);
app.use('/unprotectedContent', unprotectedContentRouter);
app.use('/unprotectedRestFunction', testRestFunctionRouter);
app.use('/protectedRestFunction', protectedRestFunctionRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
