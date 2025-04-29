SRC_DIR = src
INC_DIR = include
CC = gcc
C_FLAGS = -fsanitize=address -g -I$(INC_DIR)

OBJS = actions.o hashmap.o main.o helper_methods.o structures.o queries.o

all:	witchertracker

actions.o:	$(SRC_DIR)/actions.c $(INC_DIR)/hashmap.h $(INC_DIR)/structures.h
			$(CC) $(C_FLAGS) -c $(SRC_DIR)/actions.c -o actions.o

queries.o:	$(SRC_DIR)/queries.c $(INC_DIR)/hashmap.h $(INC_DIR)/structures.h
			$(CC) $(C_FLAGS) -c $(SRC_DIR)/queries.c -o queries.o
		
hashmap.o:	$(SRC_DIR)/hashmap.c
			$(CC) $(C_FLAGS) -c $(SRC_DIR)/hashmap.c -o hashmap.o

structures.o:	$(SRC_DIR)/structures.c
				$(CC) $(C_FLAGS) -c $(SRC_DIR)/structures.c -o structures.o

helper_methods.o:		$(SRC_DIR)/helper_methods.c $(INC_DIR)/structures.h $(INC_DIR)/hashmap.h
						$(CC) $(C_FLAGS) -c $(SRC_DIR)/helper_methods.c -o helper_methods.o

main.o: 	$(SRC_DIR)/main.c $(INC_DIR)/hashmap.h $(INC_DIR)/structures.h $(INC_DIR)/actions.h $(INC_DIR)/queries.h $(INC_DIR)/helper_methods.h
			$(CC) $(C_FLAGS) -c $(SRC_DIR)/main.c -o main.o

witchertracker: $(OBJS)
				$(CC) $(C_FLAGS) -o witchertracker $(OBJS)

grade: witchertracker
	python3 test/grader.py ./witchertracker test-cases

clean: 
	rm -f *.o witchertracker

