import os
import http.cookies
print("Content-Type: text/html\r")
print("\r")
print("<!DOCTYPE HTML><html><head><title>CGI Tester</title><meta charset=\"utf-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\" /><link rel=\"stylesheet\" href=\"../main.css\" /><noscript><link rel=\"stylesheet\" href=\"assets/css/noscript.css\" /></noscript></head><body class=\"is-preload\"><div id=\"content\">")
cookie_string = os.environ.get('HTTP_COOKIE')

if cookie_string:
    cookies = http.cookies.SimpleCookie(cookie_string)
    print("<h2>Cookies:</h2><ul>")
    cookie_keys = list(cookies.keys())
    index = 0
    while index < len(cookie_keys):
        key = cookie_keys[index]
        value = cookies[key].value
        print(f"<li>{key}: {value}</li>")
        index += 1
    print("</ul>")
else:
    print(f"<p>No cookies found</p>")
import time
import sys
sys.stdout.close()
time.sleep(31);
# print("<br/><a href=\"../\" id=\"back\">Go back</a></div></body></html>")