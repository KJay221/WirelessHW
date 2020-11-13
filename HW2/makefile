generate: car.o generate.o
	g++ -o generate.out generate.o car.o
car.o: car.cpp car.h
	g++ -c car.cpp
generate.o: generate.cpp car.h
	g++ -c generate.cpp
clean:
	rm generate.out *.o