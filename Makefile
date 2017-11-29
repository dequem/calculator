
CC=g++
CFLAGS=-I.
DEPS = calc.h
OBJ = calc.o calc_main.o
TESTS = tests.o
TESTS_DEPS = calc.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

calc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
tests: clean $(TESTS) $(DEPS) $(TESTS_DEPS)
	$(CC) -o $@ $(TESTS) $(DEPS) $(TESTS_DEPS) $(CFLAGS)
	./tests
  
clean:
	-rm -f *.o
	-rm -f tests calc

  
