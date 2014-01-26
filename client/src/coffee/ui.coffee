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
		source = $('#'+sourceId).attr 'src'
		target = $('#'+targetId).attr 'src'
		$('#'+sourceId).attr 'src', target
		$('#'+targetId).attr 'src', source


@dragOver = (event) -> event.preventDefault()
@dragEnter = (event) -> event.preventDefault()
