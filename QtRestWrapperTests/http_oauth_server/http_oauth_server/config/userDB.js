const encodedId = require("../util/encodedId");

const userDB = {
  "info@localhost": {
    password: "TEST123",
    userId: encodedId(), // incase you dont want to share the user-email.
    appPolicy: {
      sso_consumer: { role: "admin", shareEmail: true },
      simple_sso_consumer: { role: "user", shareEmail: false }
    }
  },
  "certs@example.com": {
    password: "TEST123",
    userId: encodedId(), // incase you dont want to share the user-email.
    appPolicy: {
      sso_consumer: { role: "admin", shareEmail: true },
      simple_sso_consumer: { role: "user", shareEmail: false }
    }
  }
};

module.exports = userDB;