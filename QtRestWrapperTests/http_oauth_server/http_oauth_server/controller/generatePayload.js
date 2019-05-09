const applicationCache = require("./applicationCache");
const config = require("../config");

const generatePayload = ssoToken => {
  const globalSessionToken = applicationCache.intrmTokenCache[ssoToken][0];
  const appName = applicationCache.intrmTokenCache[ssoToken][1];
  const userEmail = applicationCache.sessionUser[globalSessionToken];
  const user = config.userDB[userEmail];
  if(typeof user != typeof undefined && user != null) {
    const appPolicy = user.appPolicy[appName];
    const email = appPolicy.shareEmail === true ? userEmail : undefined;
    const payload = {
      ...{ ...appPolicy },
      ...{
        email,
        shareEmail: undefined,
        uid: user.userId,
        // global SessionID for the logout functionality.
        globalSessionID: globalSessionToken
      }
    };
    return payload;
  }
  return null;
};

module.exports = generatePayload;