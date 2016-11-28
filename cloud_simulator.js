// NOTE: the port is different
var host = "192.168.0.14", port = 3334;

var dgram = require( "dgram" );
var client = dgram.createSocket( "udp4" );
console.log("client established");
// client listens on a port as well in order to receive ping
client.bind( port, host );

// proper message sending
// NOTE: the host/port pair points at server
var message = new Buffer("Hello I'm | a packet" );

client.send(message, 0, message.length, 8000, "192.168.0.14",function(msg, rinfo ) {
    console.log( "sent message to xinu" );});

client.on("message", function( msg, rinfo ) {
    console.log( "The packet came back as"+msg+"from ip:" +rinfo.address + "and port:" + rinfo.port);
});
