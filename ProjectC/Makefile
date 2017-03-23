DEBUG = 1

SYS_BIT = 32
#SYS_BIT = `getconf LONG_BIT`

#file type
FILETYPE = c

#tool chain
#CROSS = mipsel-linux-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
AR = $(CROSS)ar


ifeq ($(CROSS), )
OPENSSL_DIR = ../third_party/openssllib_x86/
JSON_DIR = ../third_party/jsonlib_x86/
else
OPENSSL_DIR = ../third_party/openssllib_mips/
JSON_DIR = ../third_party/jsonlib_mips/
endif


#include directory
INC_FLAGS = -I ./inc
INC_FLAGS += -I ../install/
INC_FLAGS += -I $(OPENSSL_DIR)include
INC_FLAGS += -I $(JSON_DIR)include

#Project name
PROJECT = $(notdir $(CURDIR))

#install directory
INSTALL_DIR = ../install/

#compile parameters
ifeq ($(DEBUG), 1)
CDEF += -D_DEBUG
endif

ifeq ($(CROSS), )
CDEF += -DHAS_CROSS=0
else
CDEF += -DHAS_CROSS=1
endif

#compile flags
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -g3
else
CFLAGS += -O2
endif


CFLAGS += -fPIC -Wall $(INC_FLAGS) $(CDEF) 
ifeq ($(CROSS), )
ifeq ($(SYS_BIT), 64)
CFLAGS += -m64
else
CFLAGS += -m32
endif
endif

#lib directory
LIBDIR = -L ./
LIBDIR += -L $(OPENSSL_DIR)lib
LIBDIR += -L $(JSON_DIR)lib

#lib which the link program need
LIBS = $(LIBDIR) -l$(PROJECT)
LIBS += -lpthread



ifeq ($(FILETYPE), cpp) 
COMPILE = $(CXX)
else
COMPILE = $(CC)
endif

COMPILE.$(FILETYPE) = $(COMPILE) $(CFLAGS) -c
LINK.$(FILETYPE) = $(COMPILE) $(CFLAGS)

#lib source
LIB_SRC = $(wildcard lib/*.$(FILETYPE))
LIB_OBJ = $(LIB_SRC:%.$(FILETYPE)=%.o)

#target source
SRC = $(wildcard *.$(FILETYPE))
SRC += $(wildcard src/*.$(FILETYPE))
OBJ = $(SRC:%.$(FILETYPE)=%.o)

#test source
TEST_SRC = $(wildcard src_test/*.$(FILETYPE))
TEST_OBJ = $(TEST_SRC:%.$(FILETYPE)=%.o)


#include source
INC = $(wildcard inc/*.h)

#lib name
LIB_TARGET = lib$(PROJECT).so
#target name
TARGET = $(PROJECT)
#test target name
TEST_TARGET = test_$(PROJECT)

.PHONY : clean all

all : lib test $(TARGET)
$(TARGET) : $(OBJ)
	$(LINK.$(FILETYPE)) -o $@ $^ $(LIBS)


lib : $(LIB_TARGET)
$(LIB_TARGET) : $(LIB_OBJ)
	$(LINK.$(FILETYPE)) -o $@ $^ -shared
	@make libinstall

test : $(TEST_TARGET)
$(TEST_TARGET) : $(TEST_OBJ)
	$(LINK.$(FILETYPE)) -o $@ $^ $(LIBS)
	

	
#$(OBJ): %.o : %.cpp
#	@$(COMPILE.cpp) -o $@ $^
	

clean:
	@rm -f $(LIB_TARGET)
	@rm -f $(LIB_OBJ)
	@rm -f $(TARGET)
	@rm -f $(OBJ)
	@rm -f $(TEST_TARGET)
	@rm -f $(TEST_OBJ)
	
cleanall:
	@make clean

#install lib an head file, may be you need write the file you want to install(eg. ProjectC.h) 
libinstall : libuninstall
	@mkdir -p $(INSTALL_DIR)
	@cp $(LIB_TARGET) inc/ProjectC.h $(INSTALL_DIR)
	
libuninstall:	
	@rm -rf $(INSTALL_DIR)$(LIB_TARGET) $(INSTALL_DIR)ProjectC.h
	
install:
	@mkdir -p $(INSTALL_DIR)	
	@cp $(LIB_TARGET) $(TARGET) $(TEST_TARGET) inc/$(PROJECT).h $(INSTALL_DIR)

uninstall:
	@rm -rf $(INSTALL_DIR)$(LIB_TARGET) $(INSTALL_DIR)$(TARGET) $(INSTALL_DIR)$(TEST_TARGET) $(INSTALL_DIR)$(PROJECT).h 

DATE=`date +%Y%m%d%H%M%S`
NAME=backup_$(DATE).tar.bz2
FILE=`ls | grep -v backup_`
backup:clean
	@echo $(DATE)
	@echo $(NAME)
#	tar cfvj $(NAME) inc lib src Makefile rmoldbackup.sh
	tar cfvj $(NAME) $(FILE)
	./rmoldbackup.sh backup
	



