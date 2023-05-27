##
## Checkfate, Virgile (Lipatant) Berrier, 2023
## Makefile
## File description:
## Makefile
##

# VARIABLES #

# Files
NAME	=	Checkfate

# Directories
SRC_DIR	=	src/
BUILDDIR	= build
IDIR	=	include
# Flags
CPPFLAGS	=	-I./$(IDIR)
CFLAGS	=	-W -Wall -Wextra
LDFLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system
GCCFLAG	=	g++ -o
FDEBUG	=	-g3

# CONVERTION RULES #

# Sources
SRC	=	\
	$(shell cat make.txt | tr '\n' ' ')										\
# Objects
OBJ = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRC))

# Objects from Sources
$(BUILDDIR)/%.o: 	$(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

all:	$(NAME)

$(NAME):	$(OBJ)
	$(GCCFLAG) $(NAME) $(OBJ) $(LDFLAGS)

debug:	$(OBJ)
	$(GCCFLAG) $(NAME) $(OBJ) $(FDEBUG)

clean:
	$(RM) -f $(OBJ)

fclean: clean
	$(RM) -f $(NAME)

re:	fclean all

.PHONY: all debug fclean clean re $(NAME)
