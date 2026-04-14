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
