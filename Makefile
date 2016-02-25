# command and config
CXX = g++
CXXFLAGS = -O2 -Wall -static -pthread
DOXYGEN = doxygen
DOXYGENCFG = $(DOC)/doxygen.cfg
CDIR = cd
MKDIR = mkdir -p
CP = cp
RM = rm -rf

# folders
DOC = doc
OBJfolder = obj
BIN = bin
TEST = test

MACHINE = $(shell uname -m)

Include = -I src/AcousticUnit -I src/UtteranceBuilder -I src/FeatureExtractor -I src/Profexor -I src/Global -I src
VPATH = src/AcousticUnit:src/UtteranceBuilder:src/FeatureExtractor:src/Profexor:src/Global:src/Pitcher:src
vpath %.o $(OBJfolder)/$(MACHINE)

# files
ProFeXorSRC = thread_util.cpp Phd.cpp Syllable.cpp Utterance.cpp DurationFeXor.cpp EnergyFeXor.cpp PauseFeXor.cpp PitchFeXor.cpp SRIPitchFeXor.cpp SRIStylizer.cpp Matrix.cpp mymath.cpp PiecewiseLinear.cpp Spline.cpp util.cpp wavIO.cpp Config.cpp FeatureSelector.cpp Profexor.cpp SelectorKimchy.cpp CubicSplineInterpolater.cpp LinearInterpolater.cpp MaxSubtraction.cpp MeanSubtraction.cpp MeanVarNorm.cpp MovingWindowMeanSub.cpp UtteranceBuilder.cpp main_ProFeXor.cpp
ProFeXorOBJ = $(addprefix obj/$(MACHINE)/,$(ProFeXorSRC:.cpp=.o))

PitcherSRC = thread_util.cpp Matrix.cpp mymath.cpp PiecewiseLinear.cpp Spline.cpp util.cpp CubicSplineInterpolater.cpp LinearInterpolater.cpp MaxSubtraction.cpp MeanSubtraction.cpp MeanVarNorm.cpp MovingWindowMeanSub.cpp wavIO.cpp main_Pitcher.cpp
PitcherOBJ = $(addprefix obj/$(MACHINE)/,$(PitcherSRC:.cpp=.o))

# rules
all: dir profexor pitcher test

debug: CXX += -DDEBUG -g
debug: dir profexor pitcher test

profexor: $(ProFeXorOBJ)
	$(CXX) $(Include) $(CXXFLAGS) -o $(BIN)/$(MACHINE)/ProFeXor $^

pitcher: $(PitcherOBJ)
	$(CXX) $(Include) $(CXXFLAGS) -o $(BIN)/$(MACHINE)/Pitcher $^

dir:
	mkdir -p $(OBJfolder)/$(MACHINE) $(BIN)/$(MACHINE)

obj/$(MACHINE)/%.o: %.cpp
	$(CXX) $(Include) -c -o $@ $<

.PHONY : test
test:
	$(CP) $(BIN)/$(MACHINE)/* $(TEST)/

.PHONY : doc
doc:
	pdflatex -output-directory=${DOC} $(DOC)/manual; pdflatex -output-directory=${DOC} $(DOC)/manual

.PHONY : doxygen
doxygen:
	${DOXYGEN} $(DOXYGENCFG)

.PHONY : clean
clean:
	$(RM) $(OBJfolder)
