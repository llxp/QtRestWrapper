const fromAuthHeaderAsBearerToken = require("./fromAuthHeaderAsBearerToken");
const appTokenFromRequest = fromAuthHeaderAsBearerToken();
const applicationCache = require("./applicationCache");
const generatePayload = require("./generatePayload");
const config = require("../config");
const { genJwtToken } = require("./jwt_helper");

const verifySsoToken = async (req, res, next) => {
  try {
    const { ssoToken, client_secret } = req.query;
    if (client_secret == null ||
        ssoToken == null) {
      return res.status(400).json({ message: "badRequest" });
    }

    if(typeof applicationCache.validTokens[ssoToken] !== typeof undefined) {
      // if the appToken is present and check if it's valid for the application
      const appName = applicationCache.validTokens[ssoToken][1];
      const globalSessionToken = applicationCache.validTokens[ssoToken][0];
      const origin = req.get('origin');

      if (typeof config.allowedOrigins[origin] !== typeof undefined) {
      // If the appToken is not equal to ssoToken given during the sso app registraion or later stage than invalid
      if (client_secret !== config.appTokenDB[appName] ||
          //appToken !== config.appTokenDB[appName] ||
          config.allowedOrigins[origin] !== true) {
        return res.status(403).json({ message: "Unauthorized" });
      }

      return res.status(200).json({ status: "Ok" });
    } else {
      return res.status(403).json({ message: "wrong origin" });
    }
    } else {
      return res.status(403).json({ message: "Unauthorized" });
    }
  } catch(error) {
    console.log(error);
    return res.status(500).json({ message: "failure" });
  }
};

module.exports = verifySsoToken;