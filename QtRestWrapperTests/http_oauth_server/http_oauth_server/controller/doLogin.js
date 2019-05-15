const encodedId = require("../util/encodedId");
const config = require("../config");
const URL = require("url").URL;
const applicationCache = require("./applicationCache");

const doLogin = (req, res, next) => {
  // do the validation with email and password
  const { email, password } = req.body;
  if (!(config.userDB[email] && password === config.userDB[email].password)) {
    return res.status(403).json({ message: "Invalid email and password" });
  }

  // else redirect
  const { redirect_uri, result } = req.query;
  const redirect = typeof result === typeof undefined || result == null || result === 'redirect';

  const id = encodedId();
  req.session.user = id;
  applicationCache.sessionUser[id] = email;
  // redirect, if the redirect_uri is empty
  if (redirect_uri == null ||
       redirect_uri.length <= 0) {
    return redirect ? res.redirect("/") : res.status(400).json({message: 'redirect_uri is missing'});
  }
  const url = new URL(redirect_uri);
  const intrmid = encodedId();
  applicationCache.storeApplicationInCache(url.origin, id, intrmid);
  return redirect ? res.redirect(`${redirect_uri}?ssoToken=${intrmid}`) : res.status(200).json({message: 'success', ssoToken: intrmid});
};

module.exports = doLogin;