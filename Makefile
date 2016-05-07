all:
	g++ -o pattern pattern.cpp

run: 
	python pattern.py

	g++ -o pattern pattern.cpp
	./pattern

	./pattern

	python pattern.py

pythonrun:
	python pattern.py

cpprun:
	g++ -o pattern pattern.cpp
	./pattern
