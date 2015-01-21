CURRENT_COMPILER=g++

#one set for debug, another for release
CLANG_DEBUGFLAGS= -c -std=c++11 -stdlib=libstdc++ -g -DDEBUG -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions
CLANG_RELEASEFLAGS= -c -std=c++11 -stdlib=libstdc++ -fpermissive -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions 

GNU_DEBUGFLAGS= -c -std=c++11 -DSO -w -g -fpermissive -DDEBUG
GNU_RELEASEFLAGS= -c -std=c++11 -DSO -w -fpermissive 

ifeq ($(CURRENT_COMPILER), clang) 
	CURRENT_DEBUGFLAGS= $(CLANG_DEBUGFLAGS)
	CURRENT_RELEASEFLAGS= $(CLANG_RELEASEFLAGS)
endif
ifeq ($(CURRENT_COMPILER), g++)
	CURRENT_DEBUGFLAGS= $(GNU_DEBUGFLAGS)
	CURRENT_RELEASEFLAGS= $(GNU_RELEASEFLAGS)
endif

SOURCES=./source/*.cpp
INCLUDES=-I./include/
BUILDOBJECTS=./*.o

DEBUGTARGET=Debug
RELEASETARGET=Release
EXAMPLETARGET=Example
DEBUGLIBRARY_OBJECT= ./bin/libWindowAPI_D.a
RELEASELIBRARY_OBJECT= ./bin/libWindowAPI_R.a

LIBRARIES=-lGLU -lGL -lX11 -lpthread -lstdc++
ERROR_LOG=errors.txt

BUILD_DEBUGLIB=bash -c "ar rvs $(DEBUGLIBRARY_OBJECT) $(BUILDOBJECTS)"
BUILD_RELEASELIB=bash -c "ar rvs $(RELEASELIBRARY_OBJECT) $(BUILDOBJECTS)"

BUILD_EXAMPLE=bash -c "cd Example && make"

CLEAN_LIBS=bash -c "rm ./*.o"

all: $(DEBUGTARGET) $(RELEASETARGET)

$(DEBUGTARGET): $(SOURCES)
	$(CURRENT_COMPILER) $(CURRENT_DEBUGFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) 2> $(ERROR_LOG) && $(BUILD_DEBUGLIB) && $(CLEAN_LIBS)
$(RELEASETARGET): $(SOURCES)
	$(CURRENT_COMPILER) $(CURRENT_RELEASEFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) 2> $(ERROR_LOG) && $(BUILD_RELEASELIB) && $(CLEAN_LIBS)

clean: ./
	rm  $(DEBUGLIBRARY_OBJECT) $(RELEASELIBRARY_OBJECT) $(ERROR_LOG)
