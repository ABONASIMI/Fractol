/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
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

static void	parse_digits(char **s, double *res, double *frac, int frac_mode)
{
	while (**s >= '0' && **s <= '9')
	{
		if (frac_mode)
		{
			*res += (**s - '0') * (*frac);
			*frac *= 0.1;
		}
		else
			*res = *res * 10.0 + (**s - '0');
		++(*s);
	}
}

int	ft_atof_strict(char *s, double *out)
{
	double	res;
	double	frac;
	int		sign;
	int		started;

	if (!s || !*s || !out)
		return (0);
	res = 0.0;
	frac = 0.1;
	sign = 1;
	started = 0;
	if (*s == '-' || *s == '+')
		sign = -2 * (*s++ == '-') + 1;
	if (*s >= '0' && *s <= '9')
		started = 1;
	parse_digits(&s, &res, &frac, 0);
	if (*s == '.')
		++s;
	if (*s >= '0' && *s <= '9')
		started = 1;
	parse_digits(&s, &res, &frac, 1);
	if (*s || !started)
		return (0);
	*out = res * sign;
	return (1);
}

int	parse_args(int ac, char **av, t_data *data)
{
	if (ac == 2 && streq(av[1], "mandelbrot"))
		data->set = MANDELBROT;
	else if (ac == 2 && (streq(av[1], "burningship")
			|| streq(av[1], "burning_ship")))
		data->set = BURNING_SHIP;
	else if (ac == 2 && streq(av[1], "julia"))
		data->set = JULIA;
	else if (ac == 4 && streq(av[1], "julia")
		&& ft_atof_strict(av[2], &data->julia_x)
		&& ft_atof_strict(av[3], &data->julia_y))
		data->set = JULIA;
	else
		return (usage());
	set_default_view(data);
	return (1);
}
