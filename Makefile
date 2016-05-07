all:
	g++ -o pattern pattern.cpp

run: 
	g++ -o pattern pattern.cpp

	python pattern.py
	python pattern.py
	./pattern
	./pattern
	python pattern.py
	./pattern

pythonrun:
	python pattern.py

cpprun:
	g++ -o pattern pattern.cpp
	./pattern
