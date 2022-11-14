# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/11/13 01:30:19 by root          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#todo: make Makefile always call make on library makefile to make sure ITS make is upto date.
#todo: also make sure the header is up to date when constructing .o files i think
# --------------- VARIABLES ---------------#

NAME = minishell
FLAGS = -Wall -Wextra -Werror
DEBUG = -g #-fsanitize=address
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

FILES		:=	philosophers

SRCS		:=	$(addprefix $(SRC_DIR)/,$(FILES:%=%.c))
OBJS		:=	$(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:%.c=%.o)))

# -----------------------------------------#
# --------------- RECIPES -----------------#
# -----------------------------------------#

$(NAME): $(OBJS) #$(HEADER_FILES)
	gcc $(DEBUG) $(SRCS) -o $(NAME) $(INC) $(INCLUDES)

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
	./$(NAME)

.PHONY: clean fclean re test
