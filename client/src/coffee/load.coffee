# Note that most modern browsers will raise error if page address is
# other than localhost (or any other given). 
# '127.0.0.1' can cause errors as it may think it's trying to access
# resources from other web page. That is forbidden.

host = "http://localhost:3000/"
hostJSON = host + '' # TODO fill address.
# Fetch page
# TODO: error handling
@loadContent = (url, parameters) ->
	$.ajax
		type: 'POST'
		url: host + url # Will think about this later
		data: parameters
		success: (data) -> $("#content").replaceWith(data("#content"))
		async: false

	return response
			

# JSON request
# TODO: error handling
# TODO: Actually using the data.
@load = (method, requestParameters, success) ->
	parameters = [
		username: username
		method: method
		parameters: requestParameters
	]
		
	$.ajax
		type: "POST"
		url: hostJSON
		data: JSON.stringify parameters
