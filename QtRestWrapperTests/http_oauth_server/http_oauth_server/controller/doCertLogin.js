const encodedId = require("../util/encodedId");
const config = require("../config");
const URL = require("url").URL;
const applicationCache = require("./applicationCache");

const doCertLogin = (req, res, next) => {
  const { redirect_uri, result } = req.query;
  // application cache session id
  const id = encodedId();
  req.session.user = id;
  const redirect = typeof result === typeof undefined || result == null || result === 'redirect';
  // redirect, if the redirect_uri is empty
  if (redirect_uri == null ||
      redirect_uri.length <= 0) {
    return redirect ? res.redirect("/") : res.status(400).json({message: 'redirect_uri is missing'});
  }
  // get the email of the user to login in
  // from the client certificate
  const email = req.socket.getPeerCertificate().issuer.emailAddress;
  applicationCache.sessionUser[id] = email;
  const url = new URL(redirect_uri);
  // generate the temporary sso token
  const intrmid = encodedId();
  // store the temporary sso token in the cache
  applicationCache.storeApplicationInCache(url.origin, id, intrmid);
  setTimeout(function() {
    return redirect ? res.redirect(`${redirect_uri}?ssoToken=${intrmid}`) : res.status(200).json({message: 'success', ssoToken: intrmid});
  }, 5000);
  // redirect
  
};

module.exports = doCertLogin;