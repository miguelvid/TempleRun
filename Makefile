# Temple Run 3D — build
CC      := gcc
CFLAGS  := -Wall -Wextra -O2          # project code: strict warnings
VFLAGS  := -O2                        # vendored code (miniaudio): relaxed
LDFLAGS := -lglut -lGL -lGLU -lm -lpthread -ldl
INCS    := $(addprefix -I,$(shell find src -type d))

# project sources vs vendored sources (compiled with different flags)
SRC     := $(shell find src -name '*.c' -not -path 'src/vendor/*')
VSRC    := $(wildcard src/vendor/*.c)
OBJ     := $(SRC:.c=.o) $(VSRC:.c=.o)
TARGET  := templerun

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# vendored objects: relaxed flags (do not lint third-party code)
src/vendor/%.o: src/vendor/%.c
	$(CC) $(VFLAGS) $(INCS) -c $< -o $@

# project objects: strict flags
%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
