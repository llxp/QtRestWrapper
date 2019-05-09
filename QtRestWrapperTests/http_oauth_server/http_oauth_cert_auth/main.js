var fs = require('fs');
var https = require('https');
var options = {
    key: fs.readFileSync('./certs/server-key.pem'),
    cert: fs.readFileSync('./certs/server-crt.pem'),
    ca: fs.readFileSync('./certs/ca-crt.pem'),
    requestCert: true,
    rejectUnauthorized: true
};

https.createServer(options, function (req, res) {
    console.log(new Date()+' '+
        req.connection.remoteAddress+' '+
        req.socket.getPeerCertificate().subject.CN+' '+
        req.method+' '+req.url);
    res.writeHead(200);
    //res.end("hello world\n");
    res.redirect(302, "localhost:3000/simplesso/login");
}).listen(4433);