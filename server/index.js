var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

var sp = new SerialPort('/dev/ttyACM1', {
	parser: serialport.parsers.readline('\n'),
	baudrate: 115200
});

sp.on('open', function(){
	console.log('Serial port to Arduino opened');
	sp.on('data', function(data){
		console.log('data received: ', data.toString());
	});
});