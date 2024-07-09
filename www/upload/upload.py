import cgi
import os

# Directory to save uploads
UPLOAD_DIR = '/var/www/uploads'

# Ensure upload directory exists
os.makedirs(UPLOAD_DIR, exist_ok=True)

form = cgi.FieldStorage()

# Check if the file was uploaded
if 'file' not in form:
	print("Content-Type: text/html\n")
	print("<html><body><h1>No file uploaded</h1></body></html>")
	return

file_item = form['file']

if file_item.filename:
	# Create a unique file path
	file_path = os.path.join(UPLOAD_DIR, os.path.basename(file_item.filename))
	with open(file_path, 'wb') as f:
		f.write(file_item.file.read())
	
	print("Content-Type: text/html\n")
	print(f"<html><body><h1>File {file_item.filename} uploaded successfully</h1></body></html>")
else:
	print("Content-Type: text/html\n")
	print("<html><body><h1>No file uploaded</h1></body></html>")
