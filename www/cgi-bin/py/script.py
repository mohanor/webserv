from session import session_file
import os
print()

if not os.path.exists(session_file): 
    username = input()
    f = open(session_file,"w+")
    f.write(username)

print("<html>")
print("<head>")
print("<title>Cookie Example</title>")
print("</head>")
print("<body>")
print("<script>window.location.replace('http://localhost:8080/py/session.py'); console.log('hello 124')</script>")
print("</body>")
print("</html>")
