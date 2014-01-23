# Main game implementation

@callback = [] if not @callback?
@game = [] if not @game?

@callback.startGame = (data) ->
	@loadContent "game.html"
	@game.currentPlayer = new Player(data.player.name, data.player.account, data.player.cards)
	renderState data.others

@callback.refreshState = (data) ->
	renderState data.others
	setTimeout updateGame, 1000

@callback.finishGame = (data) ->
	# TODO game finalization.

renderState = (data) ->
	div = for player in data
		"<li> #{player.name}($#{player.account}) current bid: $#{player.bid} and player #{player.state} </li>"
	div.join " "
	$("#game-state").replaceWith div

updateGame = () ->
	load 'updateGame'
