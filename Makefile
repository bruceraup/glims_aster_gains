TARGETS = build

.PHONY: build all

all:
	@echo "Valid targets:   $(TARGETS)"

build:
	gcc -o glims_aster_gains glims_aster_gains.c gainutils.c -lm
