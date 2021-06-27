# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  PrEngine_config = debug
  Game_config = debug
endif
ifeq ($(config),release)
  PrEngine_config = release
  Game_config = release
endif

PROJECTS := PrEngine Game

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

PrEngine:
ifneq (,$(PrEngine_config))
	@echo "==== Building PrEngine ($(PrEngine_config)) ===="
	@${MAKE} --no-print-directory -C Game_Engine -f Makefile config=$(PrEngine_config)
endif

Game: PrEngine
ifneq (,$(Game_config))
	@echo "==== Building Game ($(Game_config)) ===="
	@${MAKE} --no-print-directory -C Game -f Makefile config=$(Game_config)
endif

clean:
	@${MAKE} --no-print-directory -C Game_Engine -f Makefile clean
	@${MAKE} --no-print-directory -C Game -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   PrEngine"
	@echo "   Game"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"