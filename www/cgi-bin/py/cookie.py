import os
import cgi
import cgitb
import http.cookies

cgitb.enable()

print("Webserv: 1.3.3.7")

headers = {}
for key, value in os.environ.items():
    if key.startswith("HTTP_"):
        headers[key[5:].title()] = value

params = cgi.FieldStorage()

cookie = http.cookies.SimpleCookie(headers.get("Cookie"))
username = cookie.get("username")
if username:
    username = username.value

if "username" in params:
    username = params["username"].value
    cookie["username"] = username
    cookie["username"]["max-age"] = 3600
    print(cookie.output())
print("\n")
print("<html>")
print("<head>")
print("<title>Cookie Example</title>")
print("</head>")
print("<body>")
if username:
    print("<p>Hello, "+ username+" !</p>")
else:
    print("<p>Please enter your name:</p>")
    print('<form method="post">')
    print('<input type="text" value="123" name="username">')
    print('<input type="submit" value="Submit">')
    print("</form>")
print("</body>")
print("</html>")
