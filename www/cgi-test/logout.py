import http.cookies
cookie = http.cookies.SimpleCookie()

cookie['username'] = "default"
cookie['description'] = "default"
cookie['age'] = 0
cookie['username']['Max-Age'] = '0'
cookie['description']['Max-Age'] = '0'
cookie['age']['Max-Age'] = '0'
print(cookie)
print("\r\n\r\n")