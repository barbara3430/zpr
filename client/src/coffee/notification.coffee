showNotification = ->
	notification = $("#notification")
	notification.is '.fly-over'
	notification.addClass 'in'

cancelNotification = ->
	notification = $("#notification")
	notification.removeClass 'in'

	if notification.hasClass 'error'
		setTimeout (->notification.removeClass 'error'), 200

	if notification.hasClass 'warning'
		setTimeout (->notification.removeClass 'warning'), 200

@notify = (message, time, state=notify.normal) ->
	notification = $("#notification")
	switch state
		when notify.error then notification.addClass 'error'
		when notify.warning then notification.addClass 'warning'
	notification.html message
	setTimeout showNotification, 200
	setTimeout cancelNotification, 200+time

@notify.normal = 0
@notify.warning = 1
@notify.error = 2
