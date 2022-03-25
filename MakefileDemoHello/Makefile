TGT = main
CC = g++
OBJ_DIR = obj
DEP_DIR = dep
CREATE_DIR = obj.dir dep.dir
#SRC = main.cpp hello.cpp world.cpp
# SRC = $(notdir $(wildcard src/*.cpp))
# seach ALL
SRC = $(notdir $(foreach dir, ./src, $(wildcard $(dir)/*.cpp)))
# OBJ = obj/hello.o obj/main.o
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))
# DEP = dep/hello.d dep/main.d
DEP = $(addprefix $(DEP_DIR)/,$(SRC:.cpp=.d))

vpath %.h include
vpath %.cpp src

$(TGT): $(OBJ)
	$(CC) $^ -o $@ -I include

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $@ -g -I include

sinclude $(DEP)
#sinclude $(addprefix $(DEP_DIR)/,$(SRC:.cpp=.d))

$(DEP_DIR)/%.d: %.cpp | $(CREATE_DIR)
	@$(CC) -MM $< > $@.$$$$ -I include;\
	sed 's,$*.o: ,$(OBJ_DIR)/$*.o $(DEP_DIR)/$*.d: ,' $@.$$$$ > $@;\
	rm -f $@.$$$$

%.dir:
	@echo "Checking Directory $*"
	@if [ ! -d $* ]; then \
		echo "Making Directory $*"; \
		mkdir -p $* ;\
	fi;

cleanall:
	rm -rf ./$(DEP_DIR) ./$(OBJ_DIR) $(TGT)
clean:
	rm -rf $(DEP_DIR)/* $(OBJ_DIR)/* $(TGT)

.PHONY: clean cleanall