targets := app
project_dir := $(shell pwd)

all: $(targets)

app:
	$(MAKE) -C app

clean:
	$(MAKE) -C app clean

.PHONY: clean app