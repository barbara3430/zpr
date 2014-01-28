# Main game implementation

@callback = [] if not @callback?
@game = [] if not @game?

@callback.startGame = (data) =>
	@loadContent "game.html"
	@game.accountVal = data.player.account
	@game.bidVal = data.player.bid
	@game.renderPlayer data.player
	@game.refreshState data
	@game.setCards data.player.cards

@callback.setCards = (data) => @game.setCards data
@callback.refreshState = (data) => @game.refreshState data
@callback.renderPlayer = (data) => @game.renderPlayer data
@callback.finishGame = (data) => @game.finishGame data

@game.refreshState = (data) ->
	game.renderState data.others
	game.lockUI data.player.state
	setTimeout game.update, 1000

@game.lockUI = (state) ->
	unless state?
		return false
	switch state
		when 0 
			$('.control').attr 'disabled', true
		when 1
			$('#bidbutton').attr 'disabled', false
			$('#checkbutton').attr 'disabled', false
			$('#allinbutton').attr 'disabled', false
			$('#passbutton').attr 'disabled', false
		when 2
			$('#changebutton').attr 'disabled', false
	return true

@game.finishGame = (data) =>
	if data.won is true  # TODO check
		notify "You win!", 3000
	else
		notify "You loose!", 3000, notify.warning
	@loadContent 'lobby.html'
	@load 'updateNames'

@game.username = @username
@game.renderPlayer = (data) ->
	@bidVal = data.bid if data.bid?
	@accountVal = data.account if data.account?

	div = "
	<div>#{@username}:</div>
	<div> Account: #{@accountVal}</div>
	<div> Bid: #{@bidVal}</div>
	"
	$('#player-state').html div

game.renderState = (data) ->
	div = for player in data
		"<li> #{player.name}($#{player.account}) current bid: $#{player.bid}</li>"
	div.join " "
	$("#game-state").replaceWith '<ul class="list">' + div + '</ul>'

game.update = () ->
	load 'updateGame'

game.bid = () ->
	newBid = $('input[name=""]').val()
	parameters = [
		bid: newBid
	]
	load 'playerRaise', parameters

game.pass = () ->
	load 'playerFold', null
	$('.control').attr 'disabled', true

game.setCards = (data) ->
	game.cards = {}
	i = 0
	for x in data
		game.cards[++i] = x
	game.replaced = []

	i = 0
	cards = for card in data
		"<div class=\"card-border\" id=\"card-border-#{++i}\" onClick=\"game.toggleCardReplace(#{i})\">
		<div class=\"card\" id=\"card-#{i}\" onDrop=\"drop(event)\" onDragOver=\"dragOver(event)\">
		<img id=\"image-#{i}\" draggable=\"true\" onDragStart=\"drag(event)\" src=\"/cards/#{card}.png\">
		</div>
		</div>"
	cards.join ' '
	$('#cards').html cards

game.change = () ->
	parameters = [replaced: [@cards[x] for x in @replaced]]
	load 'playerChange', parameters

game.toggleCardReplace = (i) ->
	unless i in game.replaced
		game.replaced.push i
		$("#card-#{i}").addClass 'card-replace'
	else
		game.replaced = game.replaced.filter (id) -> id isnt i
		$("#card-#{i}").removeClass 'card-replace'

game.allIn = () -> load 'playerAllIn'
game.check = () -> load 'playerCheck'
