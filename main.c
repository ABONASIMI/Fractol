/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnasimi <mnasimi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 21:05:00 by mnasimi           #+#    #+#             */
/*   Updated: 2026/04/14 21:05:00 by mnasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int	put_err(char *msg)
{
	int	len;

	len = 0;
	while (msg[len])
		++len;
	write(2, msg, len);
	return (1);
}

void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->img = NULL;
	data->addr = NULL;
	data->bits_per_pixel = 0;
	data->line_length = 0;
	data->endian = 0;
	data->zoom = 1.0;
	data->shift_x = 0.0;
	data->shift_y = 0.0;
	data->julia_x = -0.744;
	data->julia_y = 0.148;
	data->color_shift = 0;
	data->set = MANDELBROT;
}

void	destroy_data(t_data *data)
{
	if (data->img && data->mlx)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
}

int	setup_graphics(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (put_err("Error: mlx_init failed\n"));
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "fract-ol");
	if (!data->win)
		return (destroy_data(data), put_err("Error: mlx_new_window failed\n"));
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		return (destroy_data(data), put_err("Error: mlx_new_image failed\n"));
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (!data->addr)
		return (destroy_data(data), put_err("Error: mlx_get_data_addr failed\n"));
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	init_data(&data);
	if (!parse_args(ac, av, &data))
		return (1);
	if (setup_graphics(&data))
		return (1);
	render(&data);
	mlx_key_hook(data.win, key_hook, &data);
	mlx_mouse_hook(data.win, mouse_hook, &data);
	mlx_hook(data.win, 17, 0, close_hook, &data);
	mlx_loop(data.mlx);
	return (0);
}
