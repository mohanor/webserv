#!/usr/bin/env python

import os
import cgi
import cgitb
import http.cookies

# Enable CGI error reporting
cgitb.enable()

# Set the content type
print("Content-Type: text/html")

# Get the HTTP request headers
headers = {}
for key, value in os.environ.items():
    if key.startswith("HTTP_"):
        headers[key[5:].replace("_", "-").title()] = value

# Parse the query string parameters
params = cgi.FieldStorage()

# Get the cookie value, if it exists
cookie = http.cookies.SimpleCookie(headers.get("Cookie"))
username = cookie.get("username")
if username:
    username = username.value

# Set the cookie value
if "username" in params:
    username = params["username"].value
    cookie = http.cookies.SimpleCookie()
    cookie["username"] = username
    cookie["username"]["max-age"] = 3600  # Cookie will expire in 1 hour
    print(cookie.output())
print("\n")
# Generate the HTML response
print("<html>")
print("<head>")
print("<title>Cookie Example</title>")
print("</head>")
print("<body>")
if username:
    print("<p>Hello, {}!</p>".format(username))
else:
    print("<p>Please enter your name:</p>")
    print('<form method="post">')
    print('<input type="text" name="username">')
    print('<input type="submit" value="Submit">')
    print("</form>")
print("</body>")
print("</html>")