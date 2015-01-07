var expect = require('chai').expect;
var mod = require('./index.js');

describe('Sign-in', function(){
	it('should find and sign user in/out', function(done){
		var sp = {};
		var busyWrite = 0;
		var readyWrite = 0;
		sp.write = function(data){
			if(data === 'ready')
				readyWrite++;
			else if(data === 'busy')
				busyWrite++;
		};
		mod.dataProcess('UID Value:  0xD6 0x8A 0x43 0x30', sp, function(){
			expect(busyWrite).to.equal(1);
			expect(readyWrite).to.equal(1);
			done();
		});
	});

	it('should not find user', function(done){
		var sp = {};
		var busyWrite = 0;
		var readyWrite = 0;
		sp.write = function(data){
			if(data === 'ready')
				readyWrite++;
			else if(data === 'busy')
				busyWrite++;
		};
		mod.dataProcess('UID Value:  0xFA 0x14 0x9E 0x8E', sp, function(){
			expect(busyWrite).to.equal(0);
			expect(readyWrite).to.equal(1);
			done();
		});
	});
});