@logIn = () ->
	login = $('input[name=login]').val()
	
	if $('input[name="remember-me"]').prop 'checked'
		setCookie("username", login, 60)
	
	parameters = [
		"login": login
	]

	load 'addPlayer', parameters, loginSuccess
	return false

loginSuccess = (data) ->
	@username = data.response
	@loadContent "/lobby.html"
	@callback.refreshNames()

$ =>
	@username = null

	login = getCookie "username"
	if login?
		$('input[name="login"]').val login
		$('input[name="remember-me"]').prop 'checked', true
	else
		notify "<h1>Welcome</h1><div>Please sign in</div>", 3000

	$('#form-signin').submit logIn
