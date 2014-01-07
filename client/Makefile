# Use this to compile client

all: core.js core.css
	@echo "Ready"

core.js:
	@echo "Making coffee..."; coffee --join ./libs/core.js --compile ./src/coffee/*.coffee;

core.css:
	@echo "Compiling stylesheet..."; lessc src/less/core.less libs/core.css
