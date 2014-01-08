#!/usr/bin/env python2
import cgi
import string
import userbase

def hello(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/plain')])
    # Create instance of FieldStorage
    form = cgi.FieldStorage(
            fp=environ['wsgi.input'],
            environ=environ,
            keep_blank_values=True)

    # Get data from fields
    first_name = form.getvalue('login')
    last_name  = form.getvalue('password')

    # get data from c++ user base
    userbase.addToBase(first_name)
    users = userbase.showBase()

    #read html of main page
    f = open('main.html')
    mainhtml = string.Template( f.read() )
    f.close()

    #fill main page with user data
    result = mainhtml.safe_substitute(name=first_name, users=users)

    return result

