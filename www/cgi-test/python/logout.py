import http.cookies
cookie = http.cookies.SimpleCookie()

cookie['username'] = "default"
cookie['description'] = "default"
cookie['age'] = 0
cookie['username']['Max-Age'] = '0'
cookie['description']['Max-Age'] = '0'
cookie['age']['Max-Age'] = '0'
cookie['username']['path'] = '/cgi-test'
cookie['description']['path'] = '/cgi-test'
cookie['age']['path'] = '/cgi-test'
print(cookie.output() +  "\r")
print("\r")