const encodedId = require("../util/encodedId");
const config = require("../config");
const URL = require("url").URL;
const applicationCache = require("./applicationCache");

const doLogin = (req, res, next) => {
  // do the validation with email and password
  // but the goal is not to do the same in this right now,
  // like checking with Datebase and all, we are skiping these section
  const { email, password } = req.body;
  if (!(config.userDB[email] && password === config.userDB[email].password)) {
    return res.status(404).json({ message: "Invalid email and password" });
  }

  // else redirect
  const { serviceURL } = req.query;
  const id = encodedId();
  req.session.user = id;
  applicationCache.sessionUser[id] = email;
  if (serviceURL == null) {
    return res.redirect("/");
  }
  const url = new URL(serviceURL);
  const intrmid = encodedId();
  applicationCache.storeApplicationInCache(url.origin, id, intrmid);
  return res.redirect(`${serviceURL}?ssoToken=${intrmid}`);
};

module.exports = doLogin;