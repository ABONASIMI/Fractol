/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnasimi <mnasimi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 21:05:00 by mnasimi           #+#    #+#             */
/*   Updated: 2026/04/14 21:05:00 by mnasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	close_hook(t_data *data)
{
	destroy_data(data);
	exit(0);
	return (0);
}

static void	zoom_at_point(t_data *data, int x, int y, double factor)
{
	double	scale_x;
	double	scale_y;
	double	mouse_re;
	double	mouse_im;

	scale_x = 4.0 / (WIDTH * data->zoom);
	scale_y = 4.0 / (HEIGHT * data->zoom);
	mouse_re = (x - WIDTH / 2.0) * scale_x + data->shift_x;
	mouse_im = (y - HEIGHT / 2.0) * scale_y + data->shift_y;
	data->zoom *= factor;
	scale_x = 4.0 / (WIDTH * data->zoom);
	scale_y = 4.0 / (HEIGHT * data->zoom);
	data->shift_x = mouse_re - (x - WIDTH / 2.0) * scale_x;
	data->shift_y = mouse_im - (y - HEIGHT / 2.0) * scale_y;
}

int	mouse_hook(int button, int x, int y, t_data *data)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return (0);
	if (button == 4)
		zoom_at_point(data, x, y, ZOOM_FACTOR);
	else if (button == 5)
		zoom_at_point(data, x, y, 1.0 / ZOOM_FACTOR);
	else
		return (0);
	render(data);
	return (0);
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
		close_hook(data);
	else if (keycode == XK_Left)
		data->shift_x -= MOVE_STEP / data->zoom;
	else if (keycode == XK_Right)
		data->shift_x += MOVE_STEP / data->zoom;
	else if (keycode == XK_Up)
		data->shift_y -= MOVE_STEP / data->zoom;
	else if (keycode == XK_Down)
		data->shift_y += MOVE_STEP / data->zoom;
	else if (keycode == XK_c || keycode == XK_C)
		data->color_shift += 2;
	else if (keycode == XK_v || keycode == XK_V)
		data->color_shift -= 2;
	else if (keycode == XK_r || keycode == XK_R)
		set_default_view(data);
	else
		return (0);
	render(data);
	return (0);
}
