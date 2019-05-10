const fromAuthHeaderAsBearerToken = require("./fromAuthHeaderAsBearerToken");
const appTokenFromRequest = fromAuthHeaderAsBearerToken();
const applicationCache = require("./applicationCache");
const generatePayload = require("./generatePayload");
const config = require("../config");
const { genJwtToken } = require("./jwt_helper");

const upgradeSsoToken = async (req, res, next) => {
  try {
    const { ssoToken, client_secret } = req.query;
    //const appToken = appTokenFromRequest(req);
    // if the application token is not present or ssoToken request is invalid
    // if the ssoToken is not present in the cache some is
    // smart.
    if (client_secret == null ||
        //appToken == null ||
        ssoToken == null ||
        applicationCache.intrmTokenCache[ssoToken] == null) {
      return res.status(400).json({ message: "badRequest" });
    }

    // if the appToken is present and check if it's valid for the application
    const appName = applicationCache.intrmTokenCache[ssoToken][1];
    const globalSessionToken = applicationCache.intrmTokenCache[ssoToken][0];
    // If the appToken is not equal to token given during the sso app registraion or later stage than invalid
    if (client_secret !== config.appTokenDB[appName] ||
        //appToken !== config.appTokenDB[appName] ||
        applicationCache.sessionApp[globalSessionToken][appName] !== true) {
      return res.status(403).json({ message: "Unauthorized" });
    }
    // checking if the token passed has been generated
    const payload = generatePayload(ssoToken);
    if(payload != null) {
      const token = await genJwtToken(payload);
      const origin = req.get('origin');
      applicationCache.storeBearerTokenInCache(origin, globalSessionToken, token);
      // delete the itremCache key for no futher use,
      delete applicationCache.intrmTokenCache[ssoToken];
      return res.status(200).json({ token });
    }
    return res.status(403).json({ message: "Unauthorized" });
  } catch(error) {
    console.log(error);
    return res.status(403).json({ message: "Unauthorized" });
  }
};

module.exports = upgradeSsoToken;