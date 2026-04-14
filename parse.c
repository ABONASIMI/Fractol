/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnasimi <mnasimi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 21:05:00 by mnasimi           #+#    #+#             */
/*   Updated: 2026/04/14 21:05:00 by mnasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int	streq(char *a, char *b)
{
	while (*a && *b && *a == *b)
	{
		++a;
		++b;
	}
	return (*a == '\0' && *b == '\0');
}

static int	usage(void)
{
	static char	msg[] = "Usage:\n"
		"  ./fractol mandelbrot\n"
		"  ./fractol julia\n"
		"  ./fractol julia <real> <imag>\n"
		"  ./fractol burningship\n"
		"Available sets: mandelbrot | julia | burningship\n";
	int			w;

	w = write(1, msg, sizeof(msg) - 1);
	if (w < 0)
		return (0);
	return (0);
}

void	set_default_view(t_data *data)
{
	data->zoom = 1.0;
	data->color_shift = 0;
	if (data->set == JULIA)
	{
		data->shift_x = 0.0;
		data->shift_y = 0.0;
		return ;
	}
	data->shift_x = -0.5;
	data->shift_y = (data->set == BURNING_SHIP) * -0.5;
}

static int	parse_julia(int ac, char **av, t_data *data)
{
	if (!streq(av[1], "julia"))
		return (0);
	data->set = JULIA;
	if (ac == 2)
		return (1);
	if (ac != 4 || !ft_atof_strict(av[2], &data->julia_x)
		|| !ft_atof_strict(av[3], &data->julia_y))
		return (0);
	return (1);
}

int	parse_args(int ac, char **av, t_data *data)
{
	if (ac == 2 && streq(av[1], "mandelbrot"))
		data->set = MANDELBROT;
	else if (ac == 2 && (streq(av[1], "burningship")
			|| streq(av[1], "burning_ship")))
		data->set = BURNING_SHIP;
	else if (!parse_julia(ac, av, data))
		return (usage());
	set_default_view(data);
	return (1);
}
