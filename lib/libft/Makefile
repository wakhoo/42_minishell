# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/03 08:30:41 by grlefebv          #+#    #+#              #
#    Updated: 2025/02/14 17:19:47 by chajeon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS =  ft_isalpha.c\
		ft_isdigit.c\
		ft_isalnum.c\
		ft_isascii.c\
		ft_isprint.c\
		ft_strlen.c\
		ft_memset.c\
		ft_bzero.c\
		ft_memcpy.c\
		ft_memmove.c\
		ft_strlcpy.c\
		ft_strlcat.c\
		ft_toupper.c\
		ft_tolower.c\
		ft_strchr.c\
		ft_strrchr.c\
		ft_strncmp.c\
		ft_memchr.c\
		ft_memcmp.c\
		ft_strstr.c\
		ft_strnstr.c\
		ft_atoi.c\
		ft_atoll.c\
		ft_calloc.c\
		ft_strdup.c\
		ft_substr.c\
		ft_strjoin.c\
		ft_strtrim.c\
		ft_split.c\
		ft_itoa.c\
		ft_strmapi.c\
		ft_striteri.c\
		ft_putchar_fd.c\
		ft_putstr_fd.c\
		ft_putendl_fd.c\
		ft_putnbr_fd.c\
		get_next_line.c\
		ft_debug.c\
		ft_lstnew.c\
		ft_lstadd_front_bonus.c\
		ft_lstsize.c\
		ft_lstlast.c\
		ft_lstadd_back_bonus.c\
		ft_lstdelone.c\
		ft_lstclear.c\
		ft_lstiter.c\
		ft_lstmap.c\
		ft_tab_size.c\
		ft_strsuffix.c\
		ft_strprefix.c\
		ft_strisdigit.c\
		ft_strcpy.c\
		ft_strcmp.c\
		ft_strchrcounter.c\
		ft_realloc.c\
		ft_free_tab.c\
		ft_editstr.c\
		ft_iswhitespace.c\
		ft_lstchr.c\
		ft_dup2.c
LIBC = ar rcs	
CC = cc
FLAG = -Wall -Wextra -Werror -g3

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${LIBC} ${NAME} ${OBJS}
.c.o:
	$(CC) $(FLAG) -c $(<) -o $(<:.c=.o)

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
