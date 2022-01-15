ifndef WASI_SDK_PATH
$(error Download the WASI SDK (https://github.com/WebAssembly/wasi-sdk) and set $$WASI_SDK_PATH)
endif

CC = "$(WASI_SDK_PATH)/bin/clang" --sysroot="$(WASI_SDK_PATH)/share/wasi-sysroot"
CXX = "$(WASI_SDK_PATH)/bin/clang++" --sysroot="$(WASI_SDK_PATH)/share/wasi-sysroot"

# Optional dependency from binaryen for smaller builds
WASM_OPT = wasm-opt
WASM_OPT_FLAGS = -Oz --zero-filled-memory --strip-producers

# Whether to build for debugging instead of release
DEBUG = 0

# Compilation flags
CFLAGS = -W -Wall -Wextra -Werror -Wno-unused -MMD -MP -fno-exceptions
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -O0 -g
else
	CFLAGS += -DNDEBUG -Oz -flto
endif

# Linker flags
LDFLAGS = -Wl,-zstack-size=14752,--no-entry,--import-memory -mexec-model=reactor \
	-Wl,--initial-memory=65536,--max-memory=65536,--stack-first
ifeq ($(DEBUG), 1)
	LDFLAGS += -Wl,--export-all,--no-gc-sections
else
	LDFLAGS += -Wl,--strip-all,--gc-sections,--lto-O3 -Oz
endif

OBJECTS = $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))
OBJECTS += $(patsubst res/map/%.tmx, build/%.o, $(wildcard res/map/*.tmx))
OBJECTS += $(patsubst src/%.cpp, build/%.o, $(wildcard src/*.cpp))
DEPS = $(OBJECTS:.o=.d)

ifeq ($(OS), Windows_NT)
	MKDIR_BUILD = if not exist build md build
	RMDIR = rd /s /q
else
	MKDIR_BUILD = mkdir -p build
	RMDIR = rm -rf
endif

all: build/cart.wasm

# Link cart.wasm from all object files and run wasm-opt
build/cart.wasm: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)
ifneq ($(DEBUG), 1)
ifeq (, $(shell command -v $(WASM_OPT)))
	@echo Tip: $(WASM_OPT) was not found. Install it from binaryen for smaller builds!
else
	$(WASM_OPT) $(WASM_OPT_FLAGS) $@ -o $@
endif
endif



# Currently not supported in tool
#
# Convert tiled tilesets to json
res/map/%.set.json: res/map/%.tsx
	tiled --export-tileset json $^ $@

# Convert tiled tileset json to C header
res/map/%.set.c res/map/%.set.h: res/map/%.set.json
	w4-tiled-converter tileset $^
res/map/%.set.h: res/map/%.set.c

# Convert tiled tilemaps to json
res/map/%.map.json: res/map/%.tmx
	tiled --export-map json $^ $@

# Convert tiled tilemap json to C header
res/map/%.map.c res/map/%.map.h: res/map/%.map.json
	w4-tiled-converter tilemap $^
res/map/%.map.h: res/map/%.map.c

TILEMAPS = $(patsubst res/map/%.tmx, res/map/%.map.h, $(wildcard res/map/*.tmx))
.SECONDARY: $(TILEMAPS)

TILESETS = $(patsubst res/map/%.tsx, res/map/%.set.h, $(wildcard res/map/*.tsx))
.SECONDARY: $(TILESETS)

# Compile C sources
build/%.o: src/%.c $(TILEMAPS) $(TILESETS)
	@$(MKDIR_BUILD)
	$(CC) -c $< -o $@ $(CFLAGS)

build/%.o: res/map/%.map.c $(TILEMAPS) $(TILESETS)
	@$(MKDIR_BUILD)
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile C++ sources
build/%.o: src/%.cpp
	@$(MKDIR_BUILD)
	$(CXX) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	$(RMDIR) build
	$(RMDIR) res/map/*.json
	$(RMDIR) res/map/*.h
	$(RMDIR) res/map/*.c
	$(RMDIR) res/map/*.d

-include $(DEPS)
