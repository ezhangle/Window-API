CURRENT_COMPILER=clang

#one set for debug, another for release
CLANG_DEBUGFLAGS=-std=c++11 -stdlib=libstdc++ -g -DDEBUG -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions
CLANG_RELEASEFLAGS=-std=c++11 -stdlib=libstdc++ -fpermissive -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions 

GNU_DEBUGFLAGS=-std=c++11 -DSO -w -g -fpermissive -DDEBUG
GNU_RELEASEFLAGS= -std=c++11 -DSO -w -fpermissive 

ifeq ($(CURRENT_COMPILER),clang) 
	CURRENT_DEBUGFLAGS= $(CLANG_DEBUGFLAGS)
	CURRENT_RELEASEFLAGS= $(CLANG_RELEASEFLAGS)
endif
ifeq ($(CURRENT_COMPILER), g++)
	CURRENT_DEBUGFLAGS= $(GNU_DEBUGFLAGS)
	CURRENT_RELEASEFLAGS= $(GNU_RELEASEFLAGS)
endif

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
	$(CURRENT_COMPILER) $(CURRENT_DEBUGFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) -o $(DEBUGOBJECT) 2> $(ERROR_LOG)

$(RELEASETARGET): $(SOURCES)
	$(CURRENT_COMPILER) $(CURRENT_RELEASEFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) -o $(RELEASEOBJECT) 2> $(ERROR_LOG)

clean: ./
	rm $(DEBUGOBJECT) $(RELEASEOBJECT) $(ERROR_LOG)
