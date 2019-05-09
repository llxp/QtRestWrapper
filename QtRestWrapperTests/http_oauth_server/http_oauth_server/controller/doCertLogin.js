const encodedId = require("../util/encodedId");
const config = require("../config");
const URL = require("url").URL;
const applicationCache = require("./applicationCache");

const doCertLogin = (req, res, next) => {

  // redirect
  const { serviceURL } = req.query;
  const id = encodedId();
  req.session.user = id;
  const email = req.socket.getPeerCertificate().issuer.emailAddress;
  applicationCache.sessionUser[id] = email;
  if (serviceURL == null) {
    return res.redirect("/");
  }
  const url = new URL(serviceURL);
  const intrmid = encodedId();
  applicationCache.storeApplicationInCache(url.origin, id, intrmid);
  console.log(serviceURL);
  return res.redirect(`${serviceURL}?ssoToken=${intrmid}`);
};

module.exports = doCertLogin;