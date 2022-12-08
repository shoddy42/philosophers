# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/12/08 14:53:11 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#todo: make Makefile always call make on library makefile to make sure ITS make is upto date.
#todo: also make sure the header is up to date when constructing .o files i think
# --------------- VARIABLES ---------------#


#todo: FLAGS
NAME = philo
FLAGS = -Wall -Wextra -Werror -pthread
DEBUG = -g -fsanitize=address
CC = gcc

# --------------- DIRECTORIES ------------ #

SRC_DIR		:= src
OBJ_DIR		:= obj
BIN_DIR		:= bin
INCLUDE_DIR	:= include

HEADERS_DIR	:= include
INC			:= -I include

# --------------- FILES -------------------#

HEADER_FILES:= $(INCLUDE_DIR)/philosophers.h
HEADERS		:=	$(addprefix $(INCLUDE_DIR)/, $(HEADER_FILES))

FILES		:=	philosophers gods poet utils init init_utils sleep

SRCS		:=	$(addprefix $(SRC_DIR)/,$(FILES:%=%.c))
OBJS		:=	$(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:%.c=%.o)))

MAKEFILE	:= makefile

# -----------------------------------------#
# --------------- RECIPES -----------------#
# -----------------------------------------#

$(NAME): $(OBJS) $(HEADER_FILES) $(MAKEFILE)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME) $(INC) $(INCLUDES)

debug: $(OBJS) $(HEADER_FILES) $(MAKEFILE)
	$(CC) $(DEBUG) $(SRCS) -o $(NAME) $(INC) $(INCLUDES)


fancy: $(OBJS) $(HEADER_FILES) $(MAKEFILE)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME) $(INC) $(INCLUDES)

echo:
	@echo $(SOURCES)
	@echo "\n"
	@echo $(SRCS)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $@

#add flags back into the CC
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo Creating file.
	$(CC) $(INC) $(INCLUDE_READLINE) -c $< -o $@

flags:
	gcc $(FLAGS) $(SOURCES) -lft $(INCLUDES)
	./$(NAME)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

test: $(NAME)
	./$(NAME)

pog:
	@echo ⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠟
	@echo ⣿⣿⣯⣥⣤⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣏⣀⣀⣀⡀
	@echo ⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠻⠿⠟⠉⠉⠉⢻⣿⣿⣿⡿⠟⠋⣡⣼⣿⣿⣿⡄
	@echo ⣿⣿⣿⣟⣭⣤⣶⣶⣿⣿⠃⠀⠀⢀⣀⣤⣶⣿⣿⣿⣿⡅⡀⢀⣩⣤⣤
	@echo ⣿⣿⣿⣿⣿⣿⣛⡛⠛⠛⠛⢋⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⠛⠛⠓⠠
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣦
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⢿⡿⢿⣿⣿⣿⠃
	@echo ⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣥⣄⣀⣀⠀⠀⠀⠀⠀⢰⣾⣿⣿⠏
	@echo ⠀⠀⠀⠀⠀⠀⠉⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣜⡻⠋
	@echo ⣰⣾⣷⣶⣿⣾⣖⣻⣿⣿⡿⣿⣿⣿⣿⠿⠿⠟⠛⠛⠛⠋⠉⠉⢉⡽⠃
	@echo ⣿⣿⣿⣿⣿⣿⡉⠉⠉⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⡤⠚⠉
	@echo ⠛⠛⣿⣿⣿⣿⣿⣿⣿⠉⠛⢶⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⡇
	@echo ⠠⣾⣿⣿⣿⣿⣿⠿⠟⠃⠀⠀⠀⠈⠲⣴⣦⣤⣤⣤⣶⡾⠁
	@echo ⠄⠈⠉⠻⢿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉

.PHONY: all clean fclean re test
