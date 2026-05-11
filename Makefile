CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -I./inc

SRC_DIR = src

# Общие файлы
COMMON = $(SRC_DIR)/collection.c $(SRC_DIR)/fieldinfo.c $(SRC_DIR)/mystring.c

# Программа
program.exe: $(COMMON) $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^

# Тесты
tests.exe: $(COMMON) $(SRC_DIR)/tests.c
	$(CC) $(CFLAGS) -o $@ $^

# Запуск программы
run: program.exe
	./program.exe

# Запуск тестов
test: tests.exe
	./tests.exe

# Очистка
clean:
	del /Q *.exe 2>nul || rm -f *.exe

.PHONY: run test clean
 