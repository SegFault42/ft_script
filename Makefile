# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/05/11 00:50:28 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

####################################_COLOR_#####################################
GREEN = \033[38;5;10m
GREY = \033[38;5;60m
RED = \033[38;5;9m
END = \033[0m
##################################_COMPILATION_#################################
NAME = ft_script
CC = gcc
FLAG = -Weverything
LFT = ./libft/libft.a
SRCS =	./source/main.c\
		./source/sys.c\
		./source/parsing_arg.c\

OBJS = $(SRCS:.c=.o)

INCLUDE = -I ./include \
		  -I ./libft
###########################_RELINK_MODIFY_.h####################################
RELINK = ./include/minishell.h
################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@make -s -C ./libft/
	@$(CC) $(FLAG) -o $(NAME) $(OBJS) $(LFT)
	@printf "✅  Compilation done.\n"

%.o : %.c $(RELINK) ./Makefile
	@printf " ✅                                                              \r"
	@printf "✅  $(notdir $<)\r"
	@$(CC) -c $(FLAG) $< -o $@ $(INCLUDE)

clean:
	@printf "                                                               \r"
	@printf "✅  all .o deleted\n"
	@rm -f $(OBJS)
	@make -s clean -C ./libft/

fclean:
	@printf "                                                               \r"
	@printf "✅  libft.a, all .o and ft_script deleted\n"
	@rm -f $(NAME) $(OBJS)
	@make -s fclean -C ./libft/

norme:
	norminette | grep Error

re: fclean all
