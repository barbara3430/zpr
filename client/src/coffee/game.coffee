# Main game implementation

@callback = [] if not @callback?
@game = [] if not @game?

@callback.startGame = (data) ->
	@loadContent "game.html"
	@game.currentPlayer = new Player(data.player.name, data.player.account, data.player.cards)
	@callback.renderState data.others
	game.setCards data.player.cards

@callback.refreshState = (data) ->
	game.render data.others
	setTimeout game.update, 1000

@callback.finishGame = (data) ->
	# TODO game finalization.

game.render = (data) ->
	div = for player in data
		"<li> #{player.name}($#{player.account}) current bid: $#{player.bid} and player #{player.state} </li>"
	div.join " "
	$("#game-state").replaceWith div

game.update = () ->
	load 'updateGame'

game.bid = () ->
	newBid = $('input[name=""]').val()
	parameters = [
		bid: newBid
	]
	load 'bid', parameters

game.pass = () ->
	load 'pass', null
	$('.control').attr 'disabled', true

game.setCards = (data) ->
	unless game.cards?
		$.ajax
			type: 'GET'
			url: host + 'cards.svg'
			success: (data)->game.cards = $(data)
			dataType: 'xml'
			async: false

	i = 0
	cards = for card in data
		"<div class=\"card-border\" id=\"card-border-#{++i}\"><div class=\"card\" id=\"card-#{i}\"><img src=\"/cards/#{card}.png\"></div></div>"
	cards.join ' '
	$('#cards').html cards

game.change = () ->
	# TODO: card changing
