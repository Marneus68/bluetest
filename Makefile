TARGET  := main
WARN    := -Wall
CDLAGS  := -O2 ${WARN} -lbluetooth --cflags'
LDFLAGS := -lbluetooth --libs
CC      := gcc

C_SRCS      = $(wildcard *.c)
OBJ_FILES   = $(C_SRCS:.c=.o)

%o: %c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES)

all: ${TARGET}

clean:
	rm -rf *.o ${TARGET}
