

# import os
# import cgi
# import cgitb
# import http.cookies

# # Enable CGI error reporting
# cgitb.enable()

# # Set the content type

# # Get the HTTP request headers
# headers = {}
# for key, value in os.environ.items():
#     if key.startswith("HTTP_"):
#         headers[key[5:].replace("_", "-").title()] = value

# # Create or load the session cookie
# session_cookie = http.cookies.SimpleCookie(headers.get("Cookie")).get("session")
# if session_cookie:
#     session_id = session_cookie.value
# else:
#     session_id = os.urandom(16).hex()
#     session_cookie = http.cookies.SimpleCookie()
#     session_cookie["session"] = session_id
#     # print(session_cookie.output())

# # Load the session data from disk or create a new session
# session_data = {}
# session_filename = "./" + session_id
# if os.path.exists(session_filename):
#     with open(session_filename, "r") as f:
#         session_data = http.cookies.SimpleCookie(f.read()).get_dict()

# # Save the session data to disk
# def save_session():
#     with open(session_filename, "w") as f:
#         f.write(session_cookie.output())
#         for key, value in session_data.items():
#             f.write(http.cookies.SimpleCookie({key: value}).output())

# # Parse the query string parameters
# params = cgi.FieldStorage()
# print()
# # Get the username from the session data, or from the form data if present
# username = session_data.get("username", "")
# if "username" in params:
#     username = params["username"].value
#     session_data["username"] = username
#     save_session()

# # Generate the HTML response
# print("<html>")
# print("<head>")
# print("<title>Session Example</title>")
# print("</head>")
# print("<body>")
# if username:
#     print("<p>Hello, {}!</p>".format(username))
# else:
#     print("<p>Please enter your name:</p>")
#     print('<form method="post">')
#     print('<input type="text" name="username">')
#     print('<input type="submit" value="Submit">')
#     print("</form>")
# print("</body>")
# print("</html>")

#!/usr/bin/env python




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
sessionid = cookie.get("sessionid")
if username:
    username = username.value
if sessionid:
    sessionid = sessionid.value

if "sessionid" in params:
    sessionid = params["sessionid"].value
    cookie = http.cookies.SimpleCookie()
    cookie["sessionid"] = sessionid
    # print(cookie.output())
# if "username" in params:
    username = params["username"].value
    # cookie = http.cookies.SimpleCookie()
    cookie["username"] = username
    cookie["username"]["max-age"] = 3600
    print(cookie.output())
print("\n")
print("<html>")
print("<head>")
print("<title>Cookie Example</title>")
print("</head>")
print("<body>")
if username or sessionid:
    print("<p>Hello, "+ username+" !</p>")
else:
    sessionid = str(os.urandom(16).hex())
    print(sessionid)
    print("<p>Please enter your name:</p>")
    print('<form method="post">')
    print('<input type="text" value="{}" name="sessionid">'.format(sessionid))
    print('<input type="text" value="123" name="username">')
    print('<input type="submit" value="Submit">')
    print("</form>")
print("</body>")
print("</html>")


# import cgi
# import cgitb
# import os
# import sys

# # Enable CGI error reporting
# cgitb.enable()

# # Get form data from CGI request
# form = cgi.FieldStorage()


# cookies = {}

# httpCookie = os.environ.get("HTTP_", None)
# if  httpCookie:
#     for cookie in httpCookie.split(";"):
#         (key, value) = cookie.split("=")
#         cookies[key.strip(" ")] = value

# responseHeaders = []

# name = cookies.get("name", "Unknown")
# color = cookies.get("color", "white")

# httpMethod = os.environ.get("REQUEST_METHOD")
# if httpMethod and httpMethod == "POST":
#     # Get value of name parameter from form
#     nameValue = form.getvalue('name', None)
#     if nameValue:
#         responseHeaders += ["Set-Cookie: name=%s; Path=/" % nameValue]
#         name = nameValue
#     colorValue = form.getvalue("color", None)
#     if colorValue:
#         responseHeaders += ["Set-Cookie: color=%s; Path=/" % colorValue]
#         color = colorValue



# options = ""

# for i in ["white", "black", "red", "green", "yellow", "blue"]:
#     options += "<option value=\"%s\" %s>%s</option>" % (i, "selected" if i == color else "", i.capitalize())


# body = '''
# <head>
#     <title>CGI - Cookies </title>
#     <style>
#         * {
#             background-color: %s;
#         }
#     </style>
# </head>
# <body>
#     <h1>Welcome, %s</h1>
#     <form action="/cgi/script.py" method="POST">
#         <input type="text" name="name" value="%s">
#         <select name="color">
#             %s
#         </select>
#         <input type="submit" value="Send">
#     </form>
# </body>
# </html>
# ''' % (color, name, name, options)


# # print("Content-Length: %d" % len(body), end="\r\n")
# # print("Content-Type: text/html", end="\r\n")
# # print("Status: 404", end="\r\n")

# # for header in responseHeaders:
# #     print(header, end="\r\n")
# # print(end="\r\n")

# print(body)