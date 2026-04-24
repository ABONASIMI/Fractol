NAME = fractol

SRCS = main.c events.c render.c utils.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -std=gnu89 -Wno-error=incompatible-pointer-types

MLX_PATH = minilibx-linux
MLX_LIB = $(MLX_PATH)/libmlx.a
MLX_FLAGS = $(MLX_LIB) -lXext -lX11 -lm -lbsd

all: $(NAME)

$(MLX_LIB):
	cd $(MLX_PATH) && ./configure

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: %.c fractol.h
	$(CC) $(CFLAGS) -I$(MLX_PATH) -c $< -o $@

bonus: $(NAME)

clean:
	cd $(MLX_PATH) && ./configure clean
	rm -f $(OBJS) *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re