#!/usr/bin/env python

import cgi

print("Content-type: text/html\r\n\r\n")

form = cgi.FieldStorage()

print("<html>")
print("<head>")
print("<title>Print POST Data</title>")
print("</head>")
print("<body>")

print("<h2>POST Data:</h2>")
print("<ul>")

for key in form.keys():
    values = form.getlist(key)
    for value in values:
        print(f"<li><strong>{key}</strong>: {value}</li>")

print("</ul>")

print("</body>")
print("</html>")
