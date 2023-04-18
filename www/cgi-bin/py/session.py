import os
import uuid
import http.cookies

cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
session_id = cookie.get("sessionid")
if session_id:
    session_id = session_id.value
    cookie = http.cookies.SimpleCookie()
    cookie["sessionid"] = session_id
    print(cookie.output())
else:
    session_id = str(uuid.uuid4())
    cookie = http.cookies.SimpleCookie()
    cookie["sessionid"] = session_id
    print(cookie.output())
print()
session_file = './sessions/session_' + session_id 

def read_variables(filename):
    variables = {}
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            key, value = line.split('=', 1)
            variables[key.strip()] = value.strip()
    return variables

if os.path.exists(session_file): 
    session_data = read_variables(session_file)
else :
    session_data = {}

if __name__ == "__main__":
    if 'username' not in session_data:
        print("<html>")
        print("<head>")
        print("<title>Cookie Example</title>")
        print("</head>")
        print("<body>")
        print("<p>Please enter your name:</p>")
        print('<form method="POST" action="./script.py">')
        print('<input type="text" name="username">')
        print('<input type="submit" value="Submit">')
        print("</form>")
        print("</body>")
        print("</html>")
    else :
        print("<html>")
        print("<head>")
        print("<title>Session Example</title>")
        print("</head>")
        print("<body>")
        print("<p>Hello, {}</p>".format(session_data["username"]))
        print("</body>")
        print("</html>")
