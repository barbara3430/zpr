# Functions used to handle wainting at lobby

updateNames = () -> load 'updateNames', []

@callback = [] if not @callback?
@callback.refreshNames = (data) ->
	nameList = for name in data.names
		"<li> #{name} - Ready </li>"
	names = namesList.join ' '

	$("#player-list").html names

