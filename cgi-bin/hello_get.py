#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 
import string
import cStringIO
import dl
import userbase

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

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

print "Content-type:text/html\r\n\r\n"
print result

