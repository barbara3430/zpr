# Use this to compile client

all: folder dependencies html core.js core.css
	@echo "Ready"

html:
	@echo "Copying page templates..."; cp -R static/*  bin/

core.js:
	@echo "Making coffee..."; coffee --join ./bin/libs/core.js --compile ./src/coffee/*.coffee;

core.css:
	@echo "Compiling stylesheet..."; lessc src/less/core.less ./bin/styles/core.css

# Dirty hack, but works
folder: | bin/libs bin/styles
bin:
	@echo "Creating folder structure..."
	@mkdir -p bin
bin/libs: bin 
	@mkdir -p bin/libs
bin/styles: bin
	@mkdir -p bin/styles

dependencies: bin/libs/bootstrap.js bin/libs/jquery.js bin/styles/bootstrap.css
bin/libs/bootstrap.js:
	@cp libs/bootstrap/js/bootstrap.min.js $@
bin/libs/jquery.js:
	@cp libs/jquery/jquery.min.js $@
bin/styles/bootstrap.css:
	@cp libs/bootstrap/css/bootstrap.min.css $@

