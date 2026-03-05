CC = mpicc
CFLAGS = -Wall -O2
LDFLAGS = -lm
TARGET = distrib_grad

all: $(TARGET)

$(TARGET): distrib_grad.c
	$(CC) $(CFLAGS) -o $(TARGET) distrib_grad.c $(LDFLAGS)

run: $(TARGET)
	mpirun -np 4 ./$(TARGET)

run-scaling: $(TARGET)
	@echo "=== 1 process ==="
	mpirun -np 1 ./$(TARGET)
	@echo ""
	@echo "=== 2 processes ==="
	mpirun -np 2 ./$(TARGET)
	@echo ""
	@echo "=== 4 processes ==="
	mpirun -np 4 ./$(TARGET)
	@echo ""
	@echo "=== 8 processes ==="
	mpirun -np 8 ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run run-scaling clean
