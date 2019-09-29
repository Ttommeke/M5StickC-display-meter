
const Net = require('net');
const express = require("express");
const bodyParser = require("body-parser");

const portTcpServer = 5846;
const portHttpServer = 8080;

const server = new Net.Server();

let meters = {
    //empthy to start
    //Can be filled with a post
}

// The server listens to a socket for a client to make a connection request.
// Think of a socket as an end point.
server.listen(portTcpServer, function() {
    console.log(`Server listening for connection requests on socket localhost:${portTcpServer}.`);
});

// When a client requests a connection with the server, the server creates a new
// socket dedicated to that client.
server.on('connection', function(socket) {

    let totalrequest = "";

    // The server can also receive data from the client by reading from its socket.
    socket.on('data', function(chunk) {
        totalrequest += chunk.toString();

        if (totalrequest.indexOf('\n') != -1) {

            let request = totalrequest.split('\r\n')[0];

            let meterToReturn = meters[request];

            if (meterToReturn != undefined && meterToReturn != null) {
                socket.write(meterToReturn + "\n");
            } else {
                socket.write(-1 + "\n");
            }
        }
    });

    // When the client requests to end the TCP connection with the server, the server
    // ends the connection.
    socket.on('end', function() {
        
    });

    // Don't forget to catch error, for your own sake.
    socket.on('error', function(err) {
        console.log(`Error: ${err}`);
    });
});


const httpServer = express(portHttpServer);

httpServer.use(bodyParser.urlencoded({ extended: false }));
httpServer.use(bodyParser.json());

httpServer.get('/', function(request,response){
    response.send(meters);
});

httpServer.get('/:meterId/:meterValue', function(request,response){
    meters[request.params.meterId] = request.params.meterValue;
    response.send(meters);
});

httpServer.listen(portHttpServer, () => {
    console.log("Http server running at port " + portHttpServer);
});