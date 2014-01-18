@setCookie = (name,value,days) ->
	if days
		date = new Date()
		date.setTime date.getTime() + days*24*60*60*1000
		expires = "; expires=#{ date.toGMTString() }"
	else
		expires = ""
	document.cookie = name+"="+value+expires+"; path=/"

@getCookie = (name) ->
	cookies = document.cookie.split ';'
	for raw_cookie in cookies
		cookie = $.trim(raw_cookie)
		eq = cookie.indexOf '='
		return cookie[eq+1..] if cookie[0...eq] is name

	return null

@deleteCookie = (name) ->
	setCookie name, "", -1
