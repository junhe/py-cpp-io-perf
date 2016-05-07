targetfolder=/tmp
pythonpath=python
#pythonpath=/users/jhe//pypy-5.1.1-linux64/bin/pypy

all:
	g++ -o pattern pattern.cpp

run: 
	g++ -o pattern pattern.cpp

	${pythonpath} pattern.py ${targetfolder}
	./pattern ${targetfolder}
	./pattern ${targetfolder}
	${pythonpath} pattern.py ${targetfolder}

pythonrun:
	${pythonpath} pattern.py ${targetfolder}

pypyrun:
	${pythonpath} pattern.py ${targetfolder}

cpprun:
	g++ -o pattern pattern.cpp
	./pattern ${targetfolder}

