import http.cookies
import cgi

form = cgi.FieldStorage() 

username = form.getvalue('username')
description = form.getvalue('description')
age = form.getvalue('age')

cookie = http.cookies.SimpleCookie()

cookie['username'] = username
cookie['description'] = description
cookie['age'] = age


cookie['username']['path'] = '/cgi-test'
cookie['description']['path'] = '/cgi-test'
cookie['age']['path'] = '/cgi-test'

print(cookie)
print("\r\n\r\n")