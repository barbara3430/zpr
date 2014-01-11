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
