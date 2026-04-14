/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal_math.c                                     :+:      :+:    :+:   */
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

static int	get_iter(double zx, double zy, double cx, double cy)
{
	double	tmp;
	int		i;

	i = 0;
	while (i < MAX_ITER && (zx * zx + zy * zy) < 4.0)
	{
		tmp = zx * zx - zy * zy + cx;
		zy = 2.0 * zx * zy + cy;
		zx = tmp;
		++i;
	}
	return (i);
}

static int	get_iter_burning_ship(double cx, double cy)
{
	double	zx;
	double	zy;
	double	tmp;
	int		i;

	zx = 0.0;
	zy = 0.0;
	i = 0;
	while (i < MAX_ITER && (zx * zx + zy * zy) < 4.0)
	{
		zx = fabs(zx);
		zy = fabs(zy);
		tmp = zx * zx - zy * zy + cx;
		zy = 2.0 * zx * zy + cy;
		zx = tmp;
		++i;
	}
	return (i);
}

int	get_point_iter(t_data *data, double cx, double cy)
{
	if (data->set == JULIA)
		return (get_iter(cx, cy, data->julia_x, data->julia_y));
	if (data->set == BURNING_SHIP)
		return (get_iter_burning_ship(cx, cy));
	return (get_iter(0.0, 0.0, cx, cy));
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
