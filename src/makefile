################################ CONFIGURATION ################################

#### MAIN ######################### 
EXECUTABLE = run
H_FILES    = header.h
SOURCES    = main.c functions.c
LIBRARIES  =
FLAGS      = -pthread -fPIC -Wall 
###################################

#### SERVER ######################### 
SERVER_EXECUTABLE = server
SERVER_H_FILES    = header.h 
SERVER_SOURCES    = server.c functions.c
SERVER_LIBRARIES  =
SERVER_FLAGS      = -pthread -fPIC -Wall 
###################################


#### CLIENT ######################### 
CLIENT_EXECUTABLE = client
CLIENT_H_FILES    = header.h 
CLIENT_SOURCES    = client.c functions.c
CLIENT_LIBRARIES  =
CLIENT_FLAGS      = -pthread -fPIC -Wall 
###################################

#### COMPILER ######################### 
COMPILER_EXECUTABLE = compiler
COMPILER_H_FILES    = header.h 
COMPILER_SOURCES    = compiler.c functions.c
COMPILER_LIBRARIES  =
COMPILER_FLAGS      = -pthread -fPIC -Wall 
###################################

# RULES
## this makefile runs from inside src dir
## all .c files in ./
## all .o files in ./obj
## all .h files in ../include dir
## all local library files in ../lib

# HINTS
## eg to use the math library, set LIBRARIES = -lm
## (stackoverflow.com/questions/4606301)

############################### MAKE INSTRUCTIONS ##############################
# COMPILER
CC = gcc

# DIRECTORY VARIABLES
IDIR = ../include
ODIR = obj
LDIR = ../lib

all: $(EXECUTABLE) $(SERVER_EXECUTABLE) $(COMPILER_EXECUTABLE) $(CLIENT_EXECUTABLE) test

################ MAIN ###############################

# COMPILER FLAGS
ALL_FLAGS = -I$(IDIR) $(FLAGS)

# CREATE FULL PATHS FOR FILES
PATH_H_FILES = $(patsubst %,$(IDIR)/%,$(H_FILES))
_OBJ_FILES = $(SOURCES:.c=.o)
OBJ_FILES = $(patsubst %,$(ODIR)/%,$(_OBJ_FILES))

# COMPILATION INSTRUCTIONS
$(ODIR)/%.o: %.c $(PATH_H_FILES)
	$(CC) -c -o $@ $< $(ALL_FLAGS)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(ALL_FLAGS) $(LIBRARIES)

#####################################################


############### SERVER ##############################


# COMPILER FLAGS
ALL_FLAGS = -I$(IDIR) $(SERVER_FLAGS)

# CREATE FULL PATHS FOR FILES
PATH_H_FILES = $(patsubst %,$(IDIR)/%,$(SERVER_H_FILES))
_OBJ_FILES = $(SERVER_SOURCES:.c=.o)
OBJ_FILES = $(patsubst %,$(ODIR)/%,$(_OBJ_FILES))

# COMPILATION INSTRUCTIONS
$(ODIR)/%.o: %.c $(PATH_H_FILES)
	$(CC) -c -o $@ $< $(ALL_FLAGS)

$(SERVER_EXECUTABLE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(ALL_FLAGS) $(SERVER_LIBRARIES)

#####################################################


############### CLIENT ##############################


# COMPILER FLAGS
ALL_FLAGS = -I$(IDIR) $(CLIENT_FLAGS)

# CREATE FULL PATHS FOR FILES
PATH_H_FILES = $(patsubst %,$(IDIR)/%,$(CLIENT_H_FILES))
_OBJ_FILES = $(CLIENT_SOURCES:.c=.o)
OBJ_FILES = $(patsubst %,$(ODIR)/%,$(_OBJ_FILES))

# COMPILATION INSTRUCTIONS
$(ODIR)/%.o: %.c $(PATH_H_FILES)
	$(CC) -c -o $@ $< $(ALL_FLAGS)

$(CLIENT_EXECUTABLE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(ALL_FLAGS) $(CLIENT_LIBRARIES)

#####################################################


############### COMPILER ##############################


# COMPILER FLAGS
ALL_FLAGS = -I$(IDIR) $(COMPILER_FLAGS)

# CREATE FULL PATHS FOR FILES
PATH_H_FILES = $(patsubst %,$(IDIR)/%,$(COMPILER_H_FILES))
_OBJ_FILES = $(COMPILER_SOURCES:.c=.o)
OBJ_FILES = $(patsubst %,$(ODIR)/%,$(_OBJ_FILES))

# COMPILATION INSTRUCTIONS
$(ODIR)/%.o: %.c $(PATH_H_FILES)
	$(CC) -c -o $@ $< $(ALL_FLAGS)

$(COMPILER_EXECUTABLE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(ALL_FLAGS) $(COMPILER_LIBRARIES)

#####################################################


test:
	gcc test.c -o test


# CLEAN RULE
# .PHONY rule keeps make from doing something with a file named clean.
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

