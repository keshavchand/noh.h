.PHONY: clean dll dynamic static

dll: main.c test.c test.h
	gcc -shared -Wall -Werror -fPIC test.c -o test.so -DTEST_IMPLEMENTATION
	gcc -ggdb main.c -DTEST_DYNMAIC_LOADER -o main

dynamic: main.c test.c test.h
	gcc -shared -Wall -Werror -fPIC test.c -o test.so -DTEST_IMPLEMENTATION
	gcc main.c ./test.so -o main

static: main.c test.c test.h
	gcc main.c -DTEST_IMPLEMENTATION -o main

clean:
	rm test.so main
