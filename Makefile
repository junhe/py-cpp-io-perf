targetfolder=/tmp/

all:
	g++ -o pattern pattern.cpp

run: 
	g++ -o pattern pattern.cpp

	python pattern.py ${targetfolder}
	./pattern ${targetfolder}
	./pattern ${targetfolder}
	python pattern.py ${targetfolder}

pythonrun:
	python pattern.py ${targetfolder}

cpprun:
	g++ -o pattern pattern.cpp
	./pattern ${targetfolder}

