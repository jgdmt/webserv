import http.cookies
import cgi
import time
form = cgi.FieldStorage() 

username = form.getvalue('username')
description = form.getvalue('description')
age = form.getvalue('age')

cookie = http.cookies.SimpleCookie()

cookie['age'] = age
cookie['description'] = description
cookie['username'] = username


cookie['username']['path'] = '/cgi-test'
cookie['description']['path'] = '/cgi-test'
cookie['age']['path'] = '/cgi-test'

print(cookie.output() + "\r")
print("\r")

