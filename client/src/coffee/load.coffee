# Note that most modern browsers will raise error if page address is
# other than localhost (or any other given). 
# '127.0.0.1' can cause errors as it may think it's trying to access
# resources from other web page. That is forbidden.

host = "http://localhost:3000/"
hostJSON = host + '/test' # TODO fill address.
# Fetch page
# TODO: error handling
@loadContent = (url, parameters=null) ->
	$.ajax
		type: 'GET'
		url: host + url # Will think about this later
		data: parameters
		success: (data) -> $("#content").replaceWith(data)
		error: (_, __, text) -> notify "Error while connecting to the server", 2500, notify.error
		async: false

# JSON request
# TODO: error handling
# TODO: Actually using the data.
@load = (method, requestParameters) ->
	parameters = [
		username: username
		method: method
		parameters: requestParameters
	]
		
	$.ajax
		type: "POST"
		url: hostJSON
		data: JSON.stringify parameters
		success: successJSON
		error: (_, __, text) -> notify "Error while connecting to server", 2500, notify.error
		timeout: 1000
@callback = [] if not @callback?  # TODO: Handle this with exports

successJSON = (data) =>
	if data.error?
		notify data.error, 2500, notify.error
		return false

	method = data.method
	parameters = data.parameters
	if method of @callback
		@callback[method](parameters)
	else
		notify "Unknown answer from server.", 2500, notify.error
		return false

