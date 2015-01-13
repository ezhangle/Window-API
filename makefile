#need to change compiler to clang at some point
CC=clang
#one set for debug, another for release
DEBUGFLAGS=-std=c++11 -stdlib=libstdc++ -g -DDEBUG -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions
RELEASEFLAGS=-std=c++11 -stdlib=libstdc++ -fpermissive -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions
#using regex could be dangerous here
SOURCES=./source/*.cpp
INCLUDES=-I./include/
DEBUGOBJECT=WindowAPITest_D
RELEASEOBJECT=WindowAPITest_R
DEBUGTARGET=Debug
RELEASETARGET=Release
LIBRARIES=-lGLU -lGL -lX11 -lpthread -lstdc++
ERROR_LOG=errors.txt

all: $(DEBUGTARGET) $(RELEASETARGET)

$(DEBUGTARGET): $(SOURCES)
	$(CC) $(DEBUGFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) -o $(DEBUGOBJECT) 2> $(ERROR_LOG)

$(RELEASETARGET): $(SOURCES)
	$(CC) $(RELEASEFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) -o $(RELEASEOBJECT) 2> $(ERROR_LOG)

clean: ./
	rm $(DEBUGOBJECT) $(RELEASEOBJECT) $(ERROR_LOG)

#Window: ./
#	g++ -std=c++11 -DSO -w -g -fpermissive -o ../WindowAPITest ./*.h ./*.cpp -lGLU -lGL -lX11 -lXext 2> errors.txt


