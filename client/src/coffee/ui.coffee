@drag = (event) ->
	event.dataTransfer.setData "Text", event.target.id
	game.replaced = []
	for i in [1..5]
		$("#card-#{i}").removeClass 'card-replace'

@drop = (event) =>
	event.preventDefault()
	sourceId = event.dataTransfer.getData "Text"
	targetId = event.target.id

	source = sourceId.split '-'
	target = targetId.split '-'

	if source[0] is 'image' and target[0] is 'image'
		sourceSRC = $('#'+sourceId).attr 'src'
		targetSRC = $('#'+targetId).attr 'src'

		$('#'+sourceId).attr 'src', targetSRC
		$('#'+targetId).attr 'src', sourceSRC

		i = @game.cards[target[1]]
		@game.cards[target[1]] = @game.cards[source[1]]
		@game.cards[source[1]] = i


@dragOver = (event) -> event.preventDefault()
@dragEnter = (event) -> event.preventDefault()
