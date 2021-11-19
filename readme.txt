# Server
- The Server listens to a certain port (specified as a [command-line-argument](#command-line-arguments))
- When a connection from a client is initiated, the connection is accepted
- Whenever a client sends a request to the server, the server responds using a static web page `index.html`

## Performance
- The Server can handle up to 500 connections
- The Server can initiate the 500 connections and respond to them in about 0.07s

## Implementation Notes
- The Server has a timeout of 20 seconds, if no connections are initiated in 20 seconds, the server terminates



# Client

- A client is implemented that connects to the server and sends a request
- The number of connections created by this client is specified as a [command-line-argument](#command-line-arguments)
- `NUMBER_OF_CLIENTS` can be equal to 0. In this case, a web browser can be used to make a GET request at `http://localhost:{PORT}/`

## How To Run
- `cd build`
- `cmake ..`
- `make`
- `./Server PORT_NUMBER NUMBER_OF_CLIENTS`

## Command Line Arguments
- `PORT_NUMBER` is the port the server will listen to (e.g 4444)
- `NUMBER_OF_CLIENTS` is the number of connections created by the [client](#client) (by default 0)
