CC = clang
BIN = bin
NAME = memcache

CFLAGS = 

LDFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LDFLAGS += res/raylib/libraylib.a

SRC_C = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c)
OBJ_C = $(SRC_C:%.c=%.o)

.PHONY: dirs run

run: dirs exec
	./$(BIN)/$(NAME)

exec: $(OBJ_C)
	$(CC) -o $(BIN)/$(NAME) -g $(BIN)/objects/*.o $(LDFLAGS)

dirs:
	mkdir -p ./$(BIN)/objects

%.o: %.c
	$(CC) -o $(BIN)/objects/$(@F) -c -g $< $(CFLAGS)

clean:
	rm -rf $(BIN)
