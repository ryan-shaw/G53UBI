var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

var sp = new SerialPort('COM5', {
	parser: serialport.parsers.readline('\n'),
	baudrate: 115200
});

var sys = require('sys');
var addingUser = false;

var stdin = process.openStdin();
stdin.addListener('data', function(data){
	var command = data.toString();
	if(command.indexOf('adduser') === 0){
		var user = command.substring(command.indexOf(' ') + 1);
		console.log('Scan card now.');
		addingUser = true;
	}
});

sp.on('open', function(){
	console.log('Serial port to Arduino opened');
	sp.on('data', function(data){
		if(addingUser){
			if(data.indexOf('UID Value: ') === 0){
				var id = data.substring(12);
				console.log(id);
				var bytes = id.split(' ');
				var buf = new Buffer(bytes.length);
				for(var i = 0; i < bytes.length; i++){
					var byte = bytes[i].substring(2);
					console.log(byte);
					buf.write(byte, i);
				}
				console.log(buf.readInt32LE(0));
				addingUser = false;
			}
		}else{

		}
		console.log('data received: ', data.toString());
	});
});
