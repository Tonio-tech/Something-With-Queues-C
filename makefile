mytest: mqueue.o mytest.cpp
	g++ -Wall -g mytest.cpp mqueue.o -o mytest
mqueue.o: mqueue.cpp mqueue.h
	g++ -Wall -c -g mqueue.cpp

run:
	./driver

val:
	valgrind ./mytest

clean:
	rm *.o
	rm -rf driver.dSYM
	rm driver