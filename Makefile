CC=gcc
CFLAGS=-Iinclude -Wall -Werror
SRC=src/main.c src/transactions_lib.c src/budgets_lib.c
OBJ=$(SRC:.c=.o)
TARGET=FinManage

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)