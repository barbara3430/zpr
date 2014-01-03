#!/usr/bin/env python2
import cgi
import string
import userbase

def hello(environ, start_response):
        print('got request: %s' % environ)
        start_response('200 OK', [('Content-Type', 'text/plain')])
        # Create instance of FieldStorage
        form = cgi.FieldStorage(
                fp=environ['wsgi.input'],
                environ=environ,
                keep_blank_values=True)

        # Get data from fields
        first_name = form.getvalue('first_name')
        last_name  = form.getvalue('last_name')

        # get data from c++ user base
        userbase.addToBase(first_name)
        users = userbase.showBase()

        #read html of main page
        f = open('main.html')
        mainhtml = string.Template( f.read() )
        f.close()

        #fill main page with user data
        result = mainhtml.safe_substitute(name=first_name, users=users)

        print("Content-type:text/html\r\n\r\n")
        return result

