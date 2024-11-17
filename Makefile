FLAGS = -Wall -Wextra -Werror 

NAME = cub3D

INCLUDES = -I "Includes/"

SRC	 =	src/main.c src/key_press.c

LIBFT = lib/libft/libft.a

OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT) :$(SRC)
	@make -C lib/libft --silent
	@echo "libft DONE [✅]"

%.o: %.c
	@$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(NAME): $(OBJ)
	@$(CC)  $(INCLUDES) $(OBJ) $(LIBFT) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@echo "CUB3D DONE [🤡]"
clean:
	@echo "CLEAN DONE [✅]"
	@rm -f $(OBJ) 
	@make clean -C lib/libft --silent

fclean: clean
	@echo "FCLEAN DONE [✅]"
	@rm -f $(NAME)
	@make fclean -C lib/libft --silent
	
re: fclean all
	@echo "REBUILD DONE [✅]"

