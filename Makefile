# Temple Run 3D — build
CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS := -lglut -lGL -lGLU -lm
SRC     := $(shell find src -name '*.c')
INCS    := $(addprefix -I,$(shell find src -type d))
TARGET  := templerun

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
