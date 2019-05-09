const uuidv4 = require("uuid/v4");
const Hashids = require("hashids");
const URL = require("url").URL;
const { genJwtToken } = require("./jwt_helper");

const config = require("../config");

const login = require("./login");
const doLogin = require("./doLogin");
const verifySsoToken = require("./verifySsoToken");
const upgradeSsoToken = require("./upgradeSsoToken");

module.exports = {
  doLogin: doLogin,
  login: login,
  upgradeSsoToken: upgradeSsoToken,
  verifySsoToken: verifySsoToken
};