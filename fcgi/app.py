#!/usr/bin/env python2
"""
Main application module, contains dispatcher and initializes WSGI server.
"""

import re
import sys
import os

path = os.path.dirname(sys.argv[0])
path = os.path.abspath(path)
sys.path.append(path)

def init():
    """
    Function used to handle dispatching.
    Every module should be imported and added here with appropiate path.
    Path is then compiled to regex.
    """
    from path import hello

    myapp = Application()
    myapp.add_route("hello_get\.py", hello.hello)

    return myapp

class Application(object):
    """
    Class used to handle dispatching and requests.
    """
    urls = list()

    def __init__(self):
        self.root = None

    def __call__(self, environ, start_response):
        path = environ.get('PATH_INFO', '').lstrip('/')
        if not path:
            if self.root:
                return self.root(environ, start_response)
            else:
                start_response('404 NOT FOUND', [('Content-Type', 'text/plain')])
                return ['Not Found']

        for regex, callback in self.urls:
            match = re.search(regex, path)
            if match is not None:
                return callback(environ, start_response)
        else:
            start_response('404 NOT FOUND', [('Content-Type', 'text/plain')])
            return ['Not Found']

    def add_route(self, path, fun):
        """
        Decorator used to add paths to the application.
        """
        path = path.lstrip('/')
        regex = re.compile(path)

        if not path:
            self.root = fun
            return

        self.urls.append((regex, fun))

myapp = init()

if __name__ == '__main__':
    from flup.server.fcgi import WSGIServer
    WSGIServer(myapp).run()
