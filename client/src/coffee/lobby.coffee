# Functions used to handle wainting at lobby

updateNames = () -> load 'updateNames', []

@callback = [] if not @callback?

@callback.refreshNames = (data) ->
	if data
		nameList = for name in data.name
			"<li> #{name} - Ready </li>"
		names = namesList?.join ' '
		if names?
			$("#player-list").html names
	
	setTimeout updateNames, 1000

