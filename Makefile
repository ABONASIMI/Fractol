NAME = fractol

SRCS = main.c parse.c events.c render.c fractal_math.c utils.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -O3

MLX_PATH = minilibx-linux
MLX_LIB = $(MLX_PATH)/libmlx.a
MLX_FLAGS = $(MLX_LIB) -lXext -lX11 -lm

all: $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_PATH)

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: %.c fractol.h
	$(CC) $(CFLAGS) -I$(MLX_PATH) -c $< -o $@

bonus: $(NAME)

clean:
	$(MAKE) -C $(MLX_PATH) clean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
