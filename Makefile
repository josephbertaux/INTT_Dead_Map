SHELL=/bin/bash

SRC_DIR = src
OBJ_DIR = bld
EXE_DIR = exe

CXX = cpp
OBJ = o
EXE = exe

CMP = g++
FLAGS = `root-config --cflags`
INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

SRCS = $(shell find $(SRC_DIR) -name *.$(CXX))

TRG_SRCS = $(shell grep --include=\*.$(CXX) -ilR "$(SRC_DIR)" -e "int main")
TRG_OBJS = $(patsubst $(SRC_DIR)/%.$(CXX),$(OBJ_DIR)/%.$(OBJ),$(TRG_SRCS))
TRG_EXES = $(patsubst $(SRC_DIR)/%.$(CXX),$(EXE_DIR)/%.$(EXE),$(TRG_SRCS))

ALL_OBJS = $(patsubst $(SRC_DIR)/%.$(CXX),$(OBJ_DIR)/%.$(OBJ),$(SRCS))
OFF_OBJS = $(filter-out $(TRG_OBJS),$(ALL_OBJS))

all: $(TRG_EXES)

$(TRG_EXES) : $(ALL_OBJS)
	mkdir -p $(dir $@)
	$(CMP) $(FLAGS) $(INCS) -o $@ $(patsubst $(EXE_DIR)/%.$(EXE),$(OBJ_DIR)/%.$(OBJ),$@) $(OFF_OBJS) $(LIBS)

$(OBJ_DIR)/%.$(OBJ): $(SRC_DIR)/%.$(CXX)
	mkdir -p $(dir $@)
	$(CMP) $(FLAGS) $(INCS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(EXE_DIR)

.PHONY: srcs
srcs:
# list the path to each source relative to the root project directory
	 @echo $(SRCS)

.PHONY: all_objs
all_objs:
# list the path to each object relative to the root project directory
	@echo $(ALL_OBJS)

.PHONY: trg_objs
trg_objs:
# list the path to each object corresponding to a program, relative to the root project directory
	@echo $(TRG_OBJS)

.PHONY: off_objs
off_objs:
# list the path to each object not corresponding to a program, relative to the root project directory
	@echo $(OFF_OBJS)

.PHONY: trgs
trgs:
# list all target programs to make
	@echo $(TRGS)
