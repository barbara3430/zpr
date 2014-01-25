# Main game implementation

@callback = [] if not @callback?
@game = [] if not @game?

@callback.startGame = (data) ->
	@loadContent "game.html"
	@game.currentPlayer = new Player(data.player.name, data.player.account, data.player.cards)
	renderState data.others

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

game.change = () ->
	# TODO: card changing
