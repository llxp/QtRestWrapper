const config = require("../config");

// A temporary cache to store all the application that has login using the current session.
// It can be useful for variuos audit purpose
const sessionUser = {};
const sessionApp = {};

// these token are for the validation purpose
const intrmTokenCache = {};

var validTokens = [];

const fillIntrmTokenCache = (origin, id, intrmToken) => {
  intrmTokenCache[intrmToken] = [id, config.originAppNames[origin]];
};
const storeApplicationInCache = (origin, id, intrmToken) => {
  //console.log("origin, id, intrmToken: " + origin +" " + id + " " + intrmToken);
  if (sessionApp[id] == null) {
    sessionApp[id] = {
      [config.originAppNames[origin]]: true
    };
    fillIntrmTokenCache(origin, id, intrmToken);
  } else {
    sessionApp[id][config.originAppNames[origin]] = true;
    fillIntrmTokenCache(origin, id, intrmToken);
  }
  //console.log({ ...sessionApp }, { ...sessionUser }, { intrmTokenCache });
};

const fillValidTokenCache = (origin, id, bearerToken) => {
  validTokens[bearerToken] = [id, config.originAppNames[origin]];
};
const storeBearerTokenInCache = (origin, id, bearerToken) => {
  if (sessionApp[id] == null) {
    sessionApp[id] = {
      [config.originAppNames[origin]]: true
    };
    fillValidTokenCache(origin, id, bearerToken);
  } else {
    sessionApp[id][config.originAppNames[origin]] = true;
    fillValidTokenCache(origin, id, bearerToken);
  }
};

module.exports = {
  sessionUser: sessionUser,
  sessionApp: sessionApp,
  intrmTokenCache: intrmTokenCache,
  storeApplicationInCache: storeApplicationInCache,
  validTokens: validTokens,
  storeBearerTokenInCache: storeBearerTokenInCache
};