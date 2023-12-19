CC = gcc
OBJ = find_program
SRC = find_program.c

$(OBJ): $(SRC)
	$(CC) -o $(OBJ) $(SRC)

clean:
	rm -f $(OBJ)