var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

var sp = new SerialPort('/dev/ttyACM0', {
	parser: serialport.parsers.readline('\n'),
	baudrate: 115200
});

var sys = require('sys');

var stdin = process.openStdin();
stdin.addListener('data', function(data){
		
});

sp.on('open', function(){
	console.log('Serial port to Arduino opened');
	sp.on('data', function(data){
		console.log('data received: ', data.toString());
	});
});
