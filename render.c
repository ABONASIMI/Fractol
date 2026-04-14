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

static void	draw_row(t_data *data, t_render *r, int *row)
{
	int	i;

	r->x = 0;
	while (r->x < WIDTH)
	{
		r->cx = r->start_x + r->x * r->sx;
		i = get_point_iter(data, r->cx, r->cy);
		row[r->x] = get_palette_color(i, data->color_shift);
		++r->x;
	}
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
		r.cy = r.start_y + r.y * r.sy;
		row = (int *)(data->addr + r.y * data->line_length);
		draw_row(data, &r, row);
		++r.y;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
