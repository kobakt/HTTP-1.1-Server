# HTTP 1.1 Server

A multithreaded web server that implements the HTTP 1.1 standard. Designed for Linux.

Created as a student project.

## Multithreaded

The server is multithreaded and able to handle multiple concurrent requests. This was acheived by using a thread pool.

## Client

Browsers, telnet, and netcat can all be used to send and receive HTTP requests. Additionally, a provided client program can be used to interact with the server as well.

## Security

URL parsing is used to prevent dot-dot-slash attacks.

## Limitations

Does not fully implement the HTTP 1.1 standard, but can still be used with browsers nonetheless.

As a student project under tight time constraints, the projects code is not thoroughly clear, tested, and bug free.

## Future Goals

* Use C++ regular expressions to create clearer, more reliable text parsing.

* Add unit tests to ensure reliability.

* Fully implement the HTTP 1.1 standard.
