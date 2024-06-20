import os
from http.server import HTTPServer, SimpleHTTPRequestHandler

# Hard-coded port and directory
PORT = 8080
DIRECTORY = "/Users/rpaillet/Documents/Projects/Python/Computer Vision/cv_test/build/docs/html/index.html"

class CustomHTTPRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=kwargs.pop('directory'), **kwargs)

def run_server(port, directory):
    os.chdir(directory)
    handler = CustomHTTPRequestHandler
    server_address = ('', port)
    httpd = HTTPServer(server_address, handler)
    print(f"Serving HTTP on port {port} from directory {directory}")
    httpd.serve_forever()

if __name__ == "__main__":
    run_server(PORT, DIRECTORY)
