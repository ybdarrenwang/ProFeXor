# command and config
GPP = g++
GPPFlag = -O2 -Wall -static
DOXYGEN = doxygen
DOXYGENCFG = $(DOC)/doxygen.cfg
CDIR = cd
MKDIR = mkdir -p
CP = cp
RM = rm -r -f

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
ProFeXorSRC = Syllable.cpp Utterance.cpp DurationFeXor.cpp EnergyFeXor.cpp PauseFeXor.cpp PitchFeXor.cpp SRIPitchFeXor.cpp SRIStylizer.cpp Matrix.cpp mymath.cpp PiecewiseLinear.cpp Spline.cpp util.cpp wavIO.cpp Config.cpp FeatureSelector.cpp Profexor.cpp SelectorKimchy.cpp CubicSplineInterpolater.cpp LinearInterpolater.cpp MaxSubtraction.cpp MeanSubtraction.cpp MeanVarNorm.cpp MovingWindowMeanSub.cpp UtteranceBuilder.cpp ProFeXor.cpp
ProFeXorOBJ = $(addprefix obj/$(MACHINE)/,$(ProFeXorSRC:.cpp=.o))

PitcherSRC = Pitcher.cpp Matrix.cpp mymath.cpp PiecewiseLinear.cpp Spline.cpp util.cpp CubicSplineInterpolater.cpp LinearInterpolater.cpp MaxSubtraction.cpp MeanSubtraction.cpp MeanVarNorm.cpp MovingWindowMeanSub.cpp wavIO.cpp
PitcherOBJ = $(addprefix obj/$(MACHINE)/,$(PitcherSRC:.cpp=.o))

# rules
all: dir profexor pitcher test

dir:
	mkdir -p $(OBJfolder)/$(MACHINE) $(BIN)/$(MACHINE)

profexor: $(ProFeXorOBJ)
	$(GPP) $(Include) $(GPPFlag) -o $(BIN)/$(MACHINE)/ProFeXor $^

pitcher: $(PitcherOBJ)
	$(GPP) $(Include) $(GPPFlag) -o $(BIN)/$(MACHINE)/Pitcher $^

obj/$(MACHINE)/%.o: %.cpp
	$(GPP) $(Include) -c -g -o $@ $<

.PHONY : test
test:
	$(CP) $(BIN)/$(MACHINE)/* $(TEST)/

.PHONY : doc
doc:
	$(DOXYGEN) $(DOXYGENCFG)

.PHONY : clean
clean:
	$(RM) $(OBJfolder)/$(MACHINE)
