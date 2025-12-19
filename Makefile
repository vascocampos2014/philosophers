NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -rf

SRCBAZ = main init aux_functions handle_error set_state helper_philo philo_life

SRC = $(addsuffix .c, $(addprefix srcs/, $(SRCBAZ)))

OBJ_DIR = obj
OBJ = $(SRC:srcs/%.c=$(OBJ_DIR)/%.o)

OBJ_BONUS_DIR = obj/bonus

DEPS = $(OBJ:%.o=%.d) $(OBJ_BONUS:%.o=%.d)

# Compilação dos .o
$(OBJ_DIR)/%.o:	srcs/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Alvo principal
all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[32m[Philo created]\033[0m"

clean:
	@$(RM) obj/
	@echo "\033[33m[Cleaned up]\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[33m[Fully cleaned up]\033[0m"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus
