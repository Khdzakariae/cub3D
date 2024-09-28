CC = cc -Wall -Wextra -Werror 
CFLAGS = -I /usr/X11/include -g 
LDFLAGS = -L /usr/X11/lib -lmlx -framework OpenGL -framework AppKit

SRC = src/main.c src/key_press.c

INCLUDES = -I "Includes/"

NAME = cub3D

LIB = lib/libft/libft.a

all: $(LIB) $(NAME)

$(NAME):  $(SRC)
	@$(CC)  $(CFLAGS) $(SRC) $(LDFLAGS) $(INCLUDES) $(LIB) -o $(NAME)
	@echo "cub3D DONE [🤡]"


$(LIB) :
	@make -C lib/libft
	@echo "libft DONE [✅]"

clean:
	@echo "CLEAN DONE [✅]"
	@rm -f $(OBJ) 
	@make clean -C lib/libft

fclean: clean
	@echo "FCLEAN DONE [✅]"
	@rm -f $(NAME)
	@make fclean -C lib/libft

re: fclean all
	@echo "REBUILD DONE [✅]"

