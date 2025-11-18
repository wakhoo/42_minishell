# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dancel <dancel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/08 17:32:25 by dancel            #+#    #+#              #
#    Updated: 2025/02/20 18:30:10 by dancel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables de Compilation
# ============================
MINISHELL        = minishell
BONUS			 = bonus
MINIBONUS 		 = minishell_bonus
CC               = cc
CFLAGS           = -Wall -Werror -Wextra -g3

# Fichiers et bibliothèques
# ============================
LIBFT      = lib/libft/libft.a
INCLUDES   = -Iinclude -Ilib/libft
BINCLUDES   = -Iinclude_bonus -Ilib/libft
# Fichiers
# ============================
FILES = builtins/cd \
		builtins/echo \
		builtins/env \
		builtins/exit \
		builtins/export \
		builtins/pwd \
		builtins/unset \
		core/execute \
		core/execute_utils \
		core/launch_execute \
		core/main \
		core/pipe \
		core/signal \
		core/wildcards \
		core/wildcards_connect \
		parsing/before_execute \
		parsing/checker_util \
		parsing/convert \
		parsing/counter \
		parsing/err_msg \
		parsing/has_utils \
		parsing/heredoc \
		parsing/init_data \
		parsing/join \
		parsing/redirection \
		parsing/redirection_utils \
		parsing/replace_env \
		parsing/token_utils

BFILES = builtins/cd \
		builtins/echo \
		builtins/env \
		builtins/exit \
		builtins/export \
		builtins/pwd \
		builtins/unset \
		core/execute \
		core/execute_utils \
		core/launch_execute \
		core/main \
		core/pipe \
		core/signal \
		core/wildcards \
		core/wildcards_connect \
		parsing/before_execute \
		parsing/checker_util \
		parsing/convert \
		parsing/counter \
		parsing/err_msg \
		parsing/has_utils \
		parsing/heredoc \
		parsing/init_data \
		parsing/join \
		parsing/redirection \
		parsing/redirection_utils \
		parsing/replace_env \
		parsing/token_utils

# Fichiers source
# ============================
SRC_FILES = $(foreach f, $(FILES), sources/$(f).c)
OBJ_FILES = $(foreach f, $(FILES), objects/$(f).o)
BSRC_FILES = $(foreach f, $(BFILES), sources_bonus/$(f).c)
BOBJ_FILES = $(foreach f, $(BFILES), objects_bonus/$(f).o)

# Cible principale
# ============================
all: $(MINISHELL)

$(BONUS): $(MINIBONUS)

$(MINIBONUS): $(BOBJ_FILES) $(LIBFT)
	$(CC) -lreadline $(CFLAGS) $(BINCLUDES) $(BOBJ_FILES) $(LIBFT) -o $(MINIBONUS)

$(MINISHELL): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_FILES) $(LIBFT) -lreadline -o $(MINISHELL)

objects_bonus/%.o: sources_bonus/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/%.o: sources/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilation des bibliothèques
# ============================
$(LIBFT):
	@make -s -C lib/libft

# Nettoyage des fichiers
# ============================
clean:
	@make clean -s -C lib/libft
	@rm -f $(OBJ_FILES) $(BOBJ_FILES)
	@rm -rf objects objects_bonus

fclean: clean
	@make fclean -s -C lib/libft
	@rm -f minishell
	@rm -f minishell_bonus

# Rebuild
# ============================
re: fclean all

# Cibles "fantômes"
# ============================
.PHONY: all clean fclean re bonus
