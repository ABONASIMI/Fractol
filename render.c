/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnasimi <mnasimi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 21:05:00 by mnasimi           #+#    #+#             */
/*   Updated: 2026/04/14 21:05:00 by mnasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	get_color(int iter)
{
	double	t;
	int		r;
	int		g;
	int		b;

	if (iter == MAX_ITER)
		return (0x000000);
	t = (double)iter / MAX_ITER;
	r = (int)(9 * (1 - t) * t * t * t * 255);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return (r << 16 | g << 8 | b);
}

int	get_palette_color(int iter, int shift)
{
	static int	palette[MAX_ITER + 1];
	static int	ready;
	int			i;

	if (!ready)
	{
		i = 0;
		while (i <= MAX_ITER)
		{
			palette[i] = get_color(i);
			++i;
		}
		ready = 1;
	}
	if (iter == MAX_ITER)
		return (palette[MAX_ITER]);
	i = (iter + shift) % MAX_ITER;
	if (i < 0)
		i += MAX_ITER;
	return (palette[i]);
}

static void	init_iter(t_data *data, double *cx, double *cy, double z[2])
{
	if (data->set == JULIA)
	{
		z[0] = *cx;
		z[1] = *cy;
		*cx = data->julia_x;
		*cy = data->julia_y;
	}
	else
	{
		z[0] = 0.0;
		z[1] = 0.0;
	}
}

int	get_point_iter(t_data *data, double cx, double cy)
{
	double	z[2];
	double	tmp;
	int		i;

	init_iter(data, &cx, &cy, z);
	i = 0;
	while (i < MAX_ITER && (z[0] * z[0] + z[1] * z[1]) < 4.0)
	{
		if (data->set == BURNING_SHIP)
		{
			z[0] = fabs(z[0]);
			z[1] = fabs(z[1]);
		}
		tmp = z[0] * z[0] - z[1] * z[1] + cx;
		z[1] = 2.0 * z[0] * z[1] + cy;
		z[0] = tmp;
		++i;
	}
	return (i);
}

void	render(t_data *data)
{
	t_render	r;
	int			*row;

	r.sx = 4.0 / (WIDTH * data->zoom);
	r.sy = 4.0 / (HEIGHT * data->zoom);
	r.start_x = (-WIDTH / 2.0) * r.sx + data->shift_x;
	r.start_y = (-HEIGHT / 2.0) * r.sy + data->shift_y;
	r.y = 0;
	while (r.y < HEIGHT)
	{
		row = (int *)(data->addr + r.y * data->line_length);
		r.x = 0;
		while (r.x < WIDTH)
		{
			r.cx = r.start_x + r.x * r.sx;
			r.cy = r.start_y + r.y * r.sy;
			row[r.x] = get_palette_color(get_point_iter(data, r.cx, r.cy),
					data->color_shift);
			++r.x;
		}
		++r.y;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
