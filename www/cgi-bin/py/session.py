import cgi
import os
import sys

# Start the session
from http.cookies import SimpleCookie
from urllib.parse import urlencode

session_key = 'sessionid'

cookie = SimpleCookie(os.environ.get('HTTP_COOKIE'))
session_id = cookie.get(session_key)

if not session_id:
    session_id = os.urandom(16).hex()
else:
    session_id = session_id.value

session_file = 'session_' + session_id
try:
    with open(session_file, 'r') as f:
        session = dict(line.strip().split('=', 1) for line in f.readlines())
except FileNotFoundError:
    session = {}

def save_session():
    with open(session_file, 'w') as f:
        for k, v in session.items():
            f.write(f"{k}={v}\n")

# Handle the logout request
if 'logout' in cgi.FieldStorage():
    session.clear()
    save_session()
    sys.stdout.write("Set-Cookie: sessionid=; Max-Age=0\r\n")
    sys.stdout.write("Content-Type: text/html\r\n\r\n")
    sys.stdout.write("Logout successfuly <a href='/php'>go HOME</a>")
    sys.exit()

# Save the session
save_session()

# Send the response
sys.stdout.write("Content-Type: text/html\r\n\r\n")
sys.stdout.write("<html>")
sys.stdout.write("<head>")
sys.stdout.write("<title>Session Example</title>")
sys.stdout.write("</head>")
sys.stdout.write("<body>")
if 'name' in session:
    sys.stdout.write(f"<p>Hello, {session['name']}!</p>")
    sys.stdout.write("<form method='post'>")
    sys.stdout.write("<input type='hidden' name='logout' value='1'>")
    sys.stdout.write("<input type='submit' value='Logout'>")
    sys.stdout.write("</form>")
else:
    sys.stdout.write("<p>Please enter your name:</p>")
    sys.stdout.write("<form method='post'>")
    sys.stdout.write("<input type='text' name='name'>")
    sys.stdout.write("<input type='submit' value='Submit'>")
    sys.stdout.write("</form>")
sys.stdout.write("</body>")
sys.stdout.write("</html>")