CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb -fPIC -O2

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := CaveSurvey
SHAREDLIB   := _PHD_Lib.so


all: $(BIN)/$(SHAREDLIB) #$(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

$(BIN)/$(SHAREDLIB): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES) -shared

clean:
	-rm $(BIN)/*