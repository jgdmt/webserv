import cgi
import os
import sys

# Directory to save uploads
UPLOAD_DIR = '/Users/jgoudema/Documents/webserv/www/upload'

# Ensure upload directory exists
os.makedirs(UPLOAD_DIR, exist_ok=True)

def main():
    form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ, keep_blank_values=True)
    
    # Check if the file was uploaded
    if 'file' not in form:
        print("Content-Type: text/html\r\n\r")
        print("<html><body><h1>No file uploaded</h1></body></html>")
        return

    file_item = form['file']

    if file_item.filename:
        # Create a unique file path
        file_path = os.path.join(UPLOAD_DIR, os.path.basename(file_item.filename))
        with open(file_path, 'wb') as f:
            f.write(file_item.file.read())
        
        print("Content-Type: text/html\r\n\r")
        print(f"<html><body><h1>File {file_item.filename} uploaded successfully</h1></body></html>")
    else:
        print("Content-Type: text/html\r\n\r")
        print("<html><body><h1>No file uploaded</h1></body></html>")

main()