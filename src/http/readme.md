# How Does the HTTP Protocol Work?


* 1: The client device establishes a TCP connection with the server, usually on port 80.

* 2: The client sends a request with HTTP syntax and waits for the server’s response.

* 3: The server processes the request and sends the client a response that includes a status code and the requested data (if any).

* 4: The client closes the TCP connection. Before the client closes the session, phases 2 and 3 can be repeated as many times as necessary.



![Alt text](https://www.netburner.com/wp-content/uploads/2021/09/http_prot.png)

# HTTP Messages
* HTTP messages are the form of communication between the client and the server.

## There are two types of HTTP messages

* 1. HTTP Requests:Messages sent by the client


* 1. HTTP Responses: Messages sent from the server in response to HTTP requests

![Alt text](https://www.netburner.com/wp-content/uploads/2021/09/HTTP_message.png)

* Start-Line: Identifies the start of the HTTP request or response. The request contains the type and origin, and the response includes the status code and message. Both have the HTTP version used.

* Headers: These allow the exchange of additional information between the client and the server. Union of the Start-Line and Headers are known as Head.

* Empty-Line: Indicates the end of the meta-information.

* Body: Contains the data sent or received. Also known as the payload.

![Alt text](https://www.netburner.com/wp-content/uploads/2021/09/HTTP_request.png)

https://www.netburner.com/learn/an-introduction-to-http-protocol/