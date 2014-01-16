# Player class
class Player
	constructor: (@name, @account, @cards) ->
		@bid = 0
	putMoney: (val) ->
		if @account > val
			@bid += val
			@account -= val
			parameters = [
				account: @account
				bid: @bid
			]
			load 'putMoney', parameters
			return true
		else
			notify "Funds too low", 2000, notify.warning
			return false
# TODO: exchange cards
# TODO: Hide cards before initial.
