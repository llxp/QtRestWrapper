const fromAuthHeaderAsBearerToken = require("./fromAuthHeaderAsBearerToken");
const appTokenFromRequest = fromAuthHeaderAsBearerToken();
const applicationCache = require("./applicationCache");
const generatePayload = require("./generatePayload");
const config = require("../config");
const { genJwtToken } = require("./jwt_helper");

const verifySsoToken = async (req, res, next) => {
  try {
    const { token, client_secret } = req.query;
    if (client_secret == null ||
        token == null) {
      return res.status(400).json({ message: "badRequest" });
    }

    // if the appToken is present and check if it's valid for the application
    const appName = applicationCache.intrmTokenCache[token][1];
    const globalSessionToken = applicationCache.intrmTokenCache[token][0];

    // If the appToken is not equal to token given during the sso app registraion or later stage than invalid
    if (client_secret !== config.appTokenDB[appName] ||
        //appToken !== config.appTokenDB[appName] ||
        applicationCache.sessionApp[globalSessionToken][appName] !== true) {
      return res.status(403).json({ message: "Unauthorized" });
    }

    if(applicationCache.validTokens.indexOf(token) > -1) {
      return res.status(200).json({ status: "Ok" });
    }

    return res.status(403).json({ message: "Unauthorized" });
};

module.exports = verifySsoToken;