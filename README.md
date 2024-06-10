# HTTP 1.1 Server

This project is a multithreaded web server that implements the HTTP 1.1 standard designed for Linux. It was created as a student project to better understand the web and HTTP. The project also gave a clearer understanding of HTTP server design and implementation.

## Features

### Multithreaded

The server can handle multiple concurrent requests. This was acheived using a thread pool to keep the server performant and easily scalable.

### CGI Script Support

The server has support for CGI scripts. Parameters are passed as environment variables enabling different scripts to be used seamlessly.

### Client Compatability

The server can interact seemlessly with different clients including browsers, telnet, and netcat. Additionally, a provided client program can be used to interact with the server as well.

### Security

As a security measure, URL parsing is used to prevent dot-dot-slash attacks. 

## Limitations

The server does not fully implement the HTTP 1.1 standard. However it is still compatible with browsers. As a student project under tight time constraints, there may be areas that lack clarity, thorough testing, and could potentially contain bugs.

## Future Goals

* Implement text parsing using C++ regular expressions to create clearer, more reliable code.

* Add unit tests to ensure the reliability and robustness of the server.

* Fully implement the HTTP 1.1 standard to improve compatibility and functionality.
