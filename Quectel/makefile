##==========================================================================
 #                   Quectel OpenCPU
 #
 #              Copyright(c) 2012-2013 Quectel Ltd. All rights reserved.
 #
 #--------------------------------------------------------------------------
 # File Description
 # ----------------
 #      OpenCPU makefile for GCC.
 #
 #--------------------------------------------------------------------------
 #==========================================================================

#-------------------------------------------------------------------------------
# Configure GCC installation path, and GCC version.
# To execute "arm-none-eabi-gcc -v" in command line can get the current gcc version 
#-------------------------------------------------------------------------------
GCC_INSTALL_PATH=C:\Program Files (x86)\CodeSourcery\Sourcery_CodeBench_Lite_for_ARM_EABI
GCC_VERSION=4.7.2

#C_PREDEF=-D __CUSTOMER_CODE__
#-------------------------------------------------------------------------------
# Configure the Cloud Solution
#-------------------------------------------------------------------------------
#CLOUD_SOLUTION =
#GITWIZS_SOLUTION

#-------------------------------------------------------------------------------
# Configure version and out target
#-------------------------------------------------------------------------------
PLATFORM = APPGS3MD
MEMORY   = M32
VERSION  = A01
TARGET   = $(strip $(PLATFORM))$(strip $(MEMORY))$(strip $(VERSION))

#-------------------------------------------------------------------------------
# Configure the include directories
#-------------------------------------------------------------------------------
INCS =  -I $(ENV_INC) 
INCS += -I ./           \
        -I include      \
        -I ril/inc      \
        -I config   \


		



#-------------------------------------------------------------------------------
# Configure source code dirctories
#-------------------------------------------------------------------------------
SRC_DIRS=config \
	source \
	ril\src    \


#-------------------------------------------------------------------------------
# Configure source code files to compile in the source code directories
#-------------------------------------------------------------------------------
SRC_SYS=$(wildcard config/*.c)
SRC_SOURCE=$(wildcard source/*.c)
SRC_SYS_RIL=$(wildcard ril/src/*.c)


OBJS=\
	 $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_SYS))        \
	 $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_SOURCE))    \
	$(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_SYS_RIL))    \


#-------------------------------------------------------------------------------
# Configure user reference library
#-------------------------------------------------------------------------------
USERLIB=libs/gcc/app_start.lib

.PHONY: all
all:
#	$(warning <-- make all, C_PREDEF=$(C_PREDEF) -->)
	@$(MAKE) new -f makefile
	@copy /y libs\app_image_bin.cfg build\gcc\app_image_bin.cfg
include makefiledef
export GCC_INSTALL_PATH C_PREDEF OBJS USERLIB SRC_DIRS
