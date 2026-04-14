/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnasimi <mnasimi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 21:05:00 by mnasimi           #+#    #+#             */
/*   Updated: 2026/04/14 21:05:00 by mnasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "minilibx-linux/mlx.h"
# include <X11/keysym.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>

# define WIDTH 800
# define HEIGHT 800
# define MAX_ITER 100
# define ZOOM_FACTOR 1.2
# define MOVE_STEP 0.12

typedef enum e_set
{
	MANDELBROT = 0,
	JULIA = 1,
	BURNING_SHIP = 2
}	t_set;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	double	zoom;
	double	shift_x;
	double	shift_y;
	double	julia_x;
	double	julia_y;
	int		color_shift;
	t_set	set;
}	t_data;

typedef struct s_render
{
	int		x;
	int		y;
	double	cx;
	double	cy;
	double	start_x;
	double	start_y;
	double	sx;
	double	sy;
	int		*palette;
}	t_render;

/* Main / Init */
void	init_data(t_data *data);
void	destroy_data(t_data *data);
int		setup_graphics(t_data *data);

/* Parse */
int		parse_args(int ac, char **av, t_data *data);
void	set_default_view(t_data *data);

/* Render */
void	render(t_data *data);
int		get_color(int iter);
int		get_point_iter(t_data *data, double cx, double cy);
int		get_palette_color(int iter, int shift);

/* Events */
int		key_hook(int keycode, t_data *data);
int		mouse_hook(int button, int x, int y, t_data *data);
int		close_hook(t_data *data);

/* Utils */
int		ft_atof_strict(char *s, double *out);

#endif
