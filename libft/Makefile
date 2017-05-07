# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/04/10 04:58:10 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[48;5;9m
GREEN = \033[48;5;10m
LOL = \033[48;5;106m
LIL = \033[48;5;129m
END = \033[0m
X = 0
Y = 0

NAME = libft.a

SRC = ./ft_dprintf/sources/ft_dprintf.c\
	  ./ft_dprintf/sources/percent_c.c\
	  ./ft_dprintf/sources/percent_d.c\
	  ./ft_dprintf/sources/percent_s.c\
	  ./ft_dprintf/sources/percent_p.c\
	  ./ft_dprintf/sources/percent_x.c\
	  ./ft_dprintf/sources/percent_o.c\
	  ./ft_dprintf/sources/tools.c\
	  ./ft_print_2d_tab_no_nl.c\
	  ./ft_critical_error.c\
	  ./ft_strsplit_blank.c\
	  ./ft_print_2d_tab.c\
	  ./ft_count_2d_tab.c\
	  ./ft_str_isalpha.c\
	  ./ft_str_isspace.c\
	  ./ft_2d_tab_free.c\
	  ./ft_putendl_fd.c\
	  ./ft_putchar_fd.c\
	  ./get_next_line.c\
	  ./ft_hexa_itoa.c\
	  ./ft_hexa_ltoa.c\
	  ./ft_ltoa_base.c\
	  ./ft_putstr_fd.c\
	  ./ft_strsplit.c\
	  ./ft_memalloc.c\
	  ./ft_strccmp.c\
	  ./ft_strclen.c\
	  ./ft_strjoin.c\
	  ./ft_strncmp.c\
	  ./ft_strtrim.c\
	  ./ft_strcpy.c\
	  ./ft_memcpy.c\
	  ./ft_memset.c\
	  ./ft_strcat.c\
	  ./ft_strcmp.c\
	  ./ft_strdel.c\
	  ./ft_strlen.c\
	  ./ft_strdup.c\
	  ./ft_strchr.c\
	  ./ft_strsub.c\
	  ./ft_strnew.c\
	  ./ft_ultoa.c\
	  ./ft_debug.c\
	  ./ft_itoa.c\
	  ./ft_ltoa.c\

OBJS = $(SRC:.c=.o)

CC = clang

FLAG = -Wall -Wextra -Werror -O3
#FLAG = -Weverything -O3

HEAD = ./includes/ ./ft_dprintf/includes/ft_dprintf.h

all: $(NAME)

$(NAME): $(OBJS) ./includes/libft.h
	@$(CC) $(FLAG) -c $(SRC) -I $(HEAD)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)

%.o : %.c $(HEAD)
	@printf " ✅                                                              \r"
	@printf "✅  $(notdir $<)\r"
	$(CC) -c $(FLAG) $< -o $@

mc: fclean all clean

clean:
	@rm -f $(OBJS) ./percent_c.o ./percent_d.o ./percent_s.o ./ft_dprintf.o \
	./percent_p.o ./percent_x.o ./tools.o ./percent_o.o

fclean: clean
	@rm -f $(NAME)

re: fclean all
