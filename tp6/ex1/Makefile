CC = mpicc
CFLAGS = -Wall -O2
TARGET = matrix_transpose

all: $(TARGET)

$(TARGET): matrix_transpose.c
	$(CC) $(CFLAGS) -o $(TARGET) matrix_transpose.c

run: $(TARGET)
	mpirun -np 2 ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
