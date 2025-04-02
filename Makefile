NAME			=		scop

CXX				=		clang++
CXXFLAGS		=		-I/opt/homebrew/include -DGL_SILENCE_DEPRECATION $(shell pkg-config --cflags glfw3 glew) -g3 -std=c++17
LDFLAGS			=		-framework OpenGL $(shell pkg-config --libs glfw3 glew) -arch arm64

SRCS_DIR	= ./srcs/
SRCS		=	main.cpp			\
				Shader.cpp			\
				Model.cpp			\
				InputHandler.cpp	\
				Material.cpp
OBJS_SRCS	=	$(addprefix $(SRCS_DIR), $(SRCS:.cpp=.o))
OBJS		=	$(OBJS_SRCS)

all : $(NAME)

$(NAME) : $(OBJS)
	@echo $(CURSIVE)$(YELLOW) "      - Making $(NAME) -" $(NONE)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@
	@echo $(CURSIVE)$(YELLOW) "        - Compiling $(NAME) -" $(NONE)

%.o : %.cpp
	@echo $(CURSIVE)$(YELLOW) "      - Making object files -" $(NONE)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	@rm -fr $(OBJS)
	@echo $(CURSIVE)$(BLUE) "     - clean OBJ files -" $(NONE)

fclean : clean
	@rm -fr $(NAME)
	@echo $(CURSIVE)$(PURPLE)"      - clean $(NAME) file -"$(NONE)

re	:
	@make fclean
	@make all

.PHONY: all make clean fclean bonus re

NONE='\033[0m'
GREEN='\033[32m'
YELLOW='\033[33m'
GRAY='\033[2;37m'
CURSIVE='\033[3m'
PURPLE='\033[35m'
BLUE='\033[34m'
DELETELINE='\033[K;

