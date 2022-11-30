/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 08:27:04 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/30 01:18:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		init_philo(t_deep *thoughts)
{
	int i;
	pthread_mutex_t test;
	thoughts->philosophers = ft_calloc(thoughts->variables[NB_PHILOS], sizeof(t_phil));
	if (!thoughts->philosophers)
		exit (-1);
	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		thoughts->philosophers[i].id = i + 1;
		printf("init [%i]\n", thoughts->philosophers[i].id);
		pthread_mutex_init(&thoughts->philosophers[i].right, NULL);
		if (i > 0)
			thoughts->philosophers[i].left = thoughts->philosophers[i - 1].right;
	}
	if (i > 0)
		thoughts->philosophers[0].left = thoughts->philosophers[i - 1].right;
	return (0);
}


int		init_deepthought(int ac, char **av, t_deep *thoughts)
{
	if (ac >= 5)
	{
		thoughts->variables[NB_PHILOS] = ft_atoi(av[NB_PHILOS + 1]);
		thoughts->variables[TT_DIE]    = ft_atoi(av[TT_DIE + 1]);
		thoughts->variables[TT_EAT]    = ft_atoi(av[TT_EAT + 1]);
		thoughts->variables[TT_SLEEP]  = ft_atoi(av[TT_SLEEP + 1]);
		thoughts->variables[NB_MEALS]  = 1;
		if (ac == 6)
			thoughts->variables[NB_MEALS]  = ft_atoi(av[NB_MEALS + 1]);
	}
	else
	{
		thoughts->variables[NB_PHILOS] = DEFAULT_NB_PHILOS;
		thoughts->variables[TT_DIE]    = DEFAULT_TT_DIE;
		thoughts->variables[TT_EAT]    = DEFAULT_TT_EAT;
		thoughts->variables[TT_SLEEP]  = DEFAULT_TT_SLEEP;
		thoughts->variables[NB_MEALS]  = DEFAULT_NB_MEALS;
	}
	if (thoughts->debuglevel > 99)
	{
		printf ("philos count  = [%i]\n", thoughts->variables[NB_PHILOS]);
		printf ("time to die   = [%i]\n", thoughts->variables[TT_DIE]);
		printf ("time to eat   = [%i]\n", thoughts->variables[TT_EAT]);
		printf ("time to sleep = [%i]\n", thoughts->variables[TT_SLEEP]);
		printf ("meals count   = [%i]\n", thoughts->variables[NB_MEALS]);
	}
	init_philo(thoughts);
	return (0);
}