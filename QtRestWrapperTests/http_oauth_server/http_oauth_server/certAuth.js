var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const session = require("express-session");
var cookieSession = require('cookie-session');
var MemoryStore = require('memorystore')(session);

const doCertLogin = require("./controller/doCertLogin");

var app = express();

/*app.use(
  cookieSession({
    //secret: "keyboard cat",
    //resave: false,
    //saveUninitialized: true,
    //name: "session",
    //secure: true
    name: 'session',
    keys: ["keyboard cat"],
    // Cookie Options
    maxAge: 24 * 60 * 60 * 1000 // 24 hours
  })
);*/

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
        secret: "keyboard cat1",
        resave: true,
        saveUninitialized: true,
        name: "session1",
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
app.set('view engine', 'twig');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use("/certAuth", doCertLogin);
app.get("/", (req, res, next) => {
    res.redirect(302, "https://localhost:3001/");
});

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