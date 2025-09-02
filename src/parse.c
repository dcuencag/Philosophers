/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:34:28 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/01 15:35:42 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	is_unsigned_number(const char *s)
{
	size_t i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+')
	{
		i++;
		if (!s[i])
			return (0);
	}
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_ull(const char *s, unsigned long long *out)
{
	unsigned long long v = 0;
	size_t i = 0;

	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		unsigned int d = (unsigned int)(s[i] - '0');
		if (v > (ULLONG_MAX - d) / 10ULL)
			return (0);
		v = v * 10ULL + d;
		i++;
	}
	*out = v;
	return (1);
}

static int	parse_int_nonneg(const char *s, int *out)
{
	unsigned long long tmp;

	if (!is_unsigned_number(s))
		return (0);
	if (!parse_ull(s, &tmp))
		return (0);
	if (tmp > (unsigned long long)INT_MAX)
		return (0);
	*out = (int)tmp;
	return (1);
}

static int	validate_business_rules(t_cfg *c, int have_meals_target)
{
	if (c->nphilo < 1)
		return (0);
	if (c->t_die <= 0 || c->t_eat <= 0 || c->t_sleep <= 0)
		return (0);
	if (have_meals_target && c->meals_target <= 0)
		return (0);
	return (1);
}

int	parse_args(int ac, char **av, t_cfg *cfg)
{
	int ok;

	if (ac != 5 && ac != 6)
		return (0);

	ok  = parse_int_nonneg(av[1], &cfg->nphilo);
	ok &= parse_int_nonneg(av[2], &cfg->t_die);
	ok &= parse_int_nonneg(av[3], &cfg->t_eat);
	ok &= parse_int_nonneg(av[4], &cfg->t_sleep);
	if (!ok)
		return (0);

	if (ac == 6)
	{
		ok = parse_int_nonneg(av[5], &cfg->meals_target);
		if (!ok)
			return (0);
	}
	else
		cfg->meals_target = -1;

	if (!validate_business_rules(cfg, ac == 6))
		return (0);
	return (1);
}
