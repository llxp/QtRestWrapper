var cpspawn = require('child_process').spawn;

var functionQueue = [];

const spawnProcess = (binary, params, exitCallback) => {
	console.log(binary + ' ' + params.join(' '));
	const proc = cpspawn(binary, params);
	proc.stdout.on('data', (data) => process.stdout.write(data.toString()));
	proc.stderr.on('data', (data) => process.stdout.write(data.toString()));
	proc.on('exit', exitCallback);
	return proc;
}

var pathBin = process.env.OPENSSL_BIN || 'openssl';

if (process.argv.length !== 4) {
	console.log("Usage: node create_certs.js -opensslPath <path to openssl binary>");
	return;
} else if(process.argv.length === 4) {
	if(process.argv[2] === '-opensslPath') {
		pathBin = process.argv[3];
	} else {
		console.log("Usage: node create_certs.js -opensslPath <path to openssl binary>");
	}
}

//console.log(pathBin);

const execOpenssl = (params) => {
	functionQueue.push(params);
	if(functionQueue.length === 1) {
		spawnProcess(pathBin, params, execNextFunction);
	}
};

const execNextFunction = (code) => {
	//console.log("------------------------------");
	console.log(`\x1b[33mChild exited with code ${code}\x1b[0m`);
	//console.log("------------------------------");
	console.log("");
	functionQueue.shift();
	if(functionQueue.length > 0) {
		spawnProcess(pathBin, functionQueue[0], execNextFunction);
	}
}

// openssl        req     -new    -x509    -days    9999    -config    ./ca.cnf    -keyout    ./ca-key.pem    -out    ./ca-crt.pem
const output = execOpenssl(['req', '-new', '-x509', '-days', '9999', '-config', './ca.cnf', '-keyout', './ca-key.pem', '-out', './ca-crt.pem']);

// openssl         genrsa    -out    ./server-key.pem    4096
// openssl         genrsa    -out    ./client1-key.pem   4096
//openssl          genrsa    -out    ./client2-key.pem   4096
execOpenssl(['genrsa', '-out', './server-key.pem', '4096']);
execOpenssl(['genrsa', '-out', './client1-key.pem', '4096']);
execOpenssl(['genrsa', '-out', './client2-key.pem', '4096']);

const generateCertAndCSR = (certName, password) => {
	const cnfFile = './' + certName + '.cnf';
	const keyFile = './' + certName + '-key.pem';
	const csrFile = './' + certName + '-csr.pem';
	const crtFile = './' + certName + '-crt.pem';
	const pw = 'pass:' + password;
	// openssl         req    -new    -config   ./server.cnf -key ./server-key.pem          -out    ./server-csr.pem
	execOpenssl(['req', '-new', '-config', cnfFile, '-key', keyFile, '-out', csrFile]);
	//         x509 -req -extfile ./server.cnf -days 999 -passin "pass:password" -in ./server-csr.pem -CA ./ca-crt.pem -CAkey ./ca-key.pem -CAcreateserial -out ./ca-crt.pem
	// openssl x509 -req -extfile ./server.cnf -days 999 -passin "pass:password" -in ./server-csr.pem -CA ./ca-crt.pem -CAkey ./ca-key.pem -CAcreateserial -out ./server-crt.pem
	execOpenssl(['x509', '-req',
		'-extfile', cnfFile,
		'-days', '9999',
		'-passin', pw,
		'-in', csrFile,
		'-CA', './ca-crt.pem',
		'-CAkey', './ca-key.pem',
		'-CAcreateserial',
		'-out', crtFile]);
};

// openssl req -new -config ./server.cnf -key ./server-key.pem -out ./server-csr.pem
// openssl x509 -req -extfile ./server.cnf -days 999 -passin "pass:password" -in ./server-csr.pem -CA ./ca-crt.pem -CAkey ./ca-key.pem -CAcreateserial -out ./server-crt.pem
generateCertAndCSR("server", "password");
// openssl req -new -config ./client1.cnf -key ./client1-key.pem -out ./client1-csr.pem
// openssl x509 -req -extfile ./client1.cnf -days 999 -passin "pass:password" -in ./client1-csr.pem -CA ./ca-crt.pem -CAkey ./ca-key.pem -CAcreateserial -out ./client1-crt.pem
generateCertAndCSR("client1", "password");
// openssl req -new -config ./client2.cnf -key ./client2-key.pem -out ./client2-csr.pem
// openssl x509 -req -extfile ./client2.cnf -days 999 -passin "pass:password" -in ./client2-csr.pem -CA ./ca-crt.pem -CAkey ./ca-key.pem -CAcreateserial -out ./client2-crt.pem
generateCertAndCSR("client2", "password");

// openssl verify -CAfile ./ca-crt.pem ./server-crt.pem
// openssl verify -CAfile ./ca-crt.pem ./client1-crt.pem
// openssl verify -CAfile ./ca-crt.pem ./client2-crt.pem
execOpenssl(['verify', '-CAfile', './ca-crt.pem', './server-crt.pem']);
execOpenssl(['verify', '-CAfile', './ca-crt.pem', './client1-crt.pem']);
execOpenssl(['verify', '-CAfile', './ca-crt.pem', './client2-crt.pem']);

// openssl pkcs12 -inkey ./client1-key.pem -in ./client1-crt.pem -export -out ./client1-crt.pfx
// openssl pkcs12 -inkey ./client2-key.pem -in ./client2-crt.pem -export -out ./client2-crt.pfx

const exportPassword = 'TEST123';
execOpenssl(['pkcs12',
	'-inkey', './client1-key.pem',
	'-in', './client1-crt.pem',
	'-export', '-password', 'pass:' + exportPassword,
	'-out', './client1-crt.pfx',
]);
execOpenssl(['pkcs12',
	'-inkey', './client2-key.pem',
	'-in', './client2-crt.pem',
	'-export', '-password', 'pass:' + exportPassword,
	'-out', './client2-crt.pfx',
]);