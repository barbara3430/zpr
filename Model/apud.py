#!/usr/bin/env python
# ["addPlayer", param1, param2]
# [{"username":null,"method":"addPlayer","parameters":[{"login":"nazwa"}]}]
# [{"username":nazwa,"method":"updateNames","parameters": null
# [{"username":nazwa,"method":"updateGame", parameters:null}]
# [{"username":nazwa,"method":"nazwa_akcji", paramethers: null albo bid}]
# addPlayer, updateNames, updateGame, playerCheck, playerFold, playerAllIn, playerRaise, playerChange
#
#   mm   mmmmm  m    m mmmm  
#   ##   #   "# #    # #   "m
#  #  #  #mmm#" #    # #    #
#  #mm#  #      #    # #    #
# #    # #      "mmmm" #mmm"  PROJECT v. 1.0
#
#
# ______
#< APUD >
# ------
#        \   ^__^
#         \  (oo)\_______
#            (__)\       )\/\
#                ||----w |
#                ||     ||

import re
import sys
import os
import json
import Model

def app(environ, start_response):
    try:
        request_body_size = int(environ.get('CONTENT_LENGTH', 0))
    except (ValueError):
        request_body_size = 0
    request_body = environ['wsgi.input'].read(request_body_size)
    p = json.loads(request_body)
    print p
    if type(p) == list:
      p = p[0]
    username = p["username"]
    methodName = p["method"]
    parameters = p["parameters"]
    params = [username] + (parameters or {}).values()
    result = getattr(Model, methodName)(*params)
      
    response_body = json.dumps(result)
    status = '200 OK'

    response_headers = [('Content-Type', 'application/json'),
                        ('Content-Length', str(len(response_body)))]
    start_response(status, response_headers)
    return [response_body]

if __name__ == '__main__':
    from flup.server.fcgi import WSGIServer
    WSGIServer(app).run()

