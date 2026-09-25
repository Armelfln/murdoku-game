CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Isrc
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
TARGET = murdoku

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

ifeq ($(UNAME_S), Linux)
    CFLAGS += $(shell pkg-config --cflags raylib)
    LDFLAGS = $(shell pkg-config --libs raylib)
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

re: clean all

.PHONY: all clean re