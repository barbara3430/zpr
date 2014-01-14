# Note that most modern browsers will raise error if page address is
# other than localhost. 
# '127.0.0.1' can cause errors as it may think it's trying to access
# resources from other web page. That is forbidden.

host = "http://localhost:3000/"
# Fetch page
# TODO: error handling
@loadURL = (url, parameters) ->
	response = null
	$.ajax
		type: 'POST'
		url: host + url # Will think about this later
		data: parameters
		success: (data) -> response = data
		async: false

	return response

# JSON request
# TODO: url parameter here may be redundant.
# TODO: error handling
# TODO: Actually using the data.
@load = (url, parameters) ->
	$.ajax
		type: "POST"
		url: host + url
		data: JSON.stringify(parameters)
