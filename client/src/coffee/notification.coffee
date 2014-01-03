showNotification = ->
	notification = $("#notification")
	notification.is '.fly-over'
	notification.addClass 'in'

cancelNotification = ->
	notification = $("#notification")
	notification.removeClass 'in'

@notify = (message, time) ->
	notification = $("#notification")
	notification.html message
	setTimeout showNotification, 200
	setTimeout cancelNotification, 200+time
