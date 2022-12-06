/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 08:27:04 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 15:20:33 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	create_threads(t_deep *thoughts)
{
	int	i;
	t_phil *philo;

	philo = thoughts->philosophers;

	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		pthread_create(&philo[i].thread, NULL, yes, (void *)&philo[i]);
	}
	pthread_create(&thoughts->shakespeare, NULL, &shakespeare, thoughts);
}

void	join_threads(t_deep *thoughts)
{
	int	i;
	t_phil *philo;

	philo = thoughts->philosophers;

	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		pthread_join(philo[i].thread, NULL);		
	}
	pthread_join(thoughts->shakespeare, NULL);
}

void	destroy_forks(t_deep *thoughts)
{
	int	i;
	t_phil *philo;

	philo = thoughts->philosophers;

	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		pthread_mutex_destroy(thoughts->philosophers->right);	
	}
	// pthread_join(thoughts->shakespeare, NULL);
}


//todo: error handling
int		init_philo(t_deep *thoughts)
{
	int i;
	thoughts->philosophers = ft_calloc(thoughts->variables[NB_PHILOS], sizeof(t_phil));
	if (!thoughts->philosophers)
		exit (-1);
	thoughts->epoch = get_time();
	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		thoughts->philosophers[i].id = i + 1;
		thoughts->philosophers[i].thoughts = thoughts;
		thoughts->philosophers[i].right = ft_calloc(1, sizeof(t_mutex));
		thoughts->philosophers[i].last_supper = thoughts->epoch;
		pthread_mutex_init(thoughts->philosophers[i].right, NULL);
		pthread_mutex_init(&thoughts->philosophers[i].soul, NULL);
		if (i > 0)
			thoughts->philosophers[i].left = thoughts->philosophers[i - 1].right;
	}
	// printf ("i = %i\n", i);
	if (i > 1)
	{
		// printf ("SET LEFT!\n");
		thoughts->philosophers[0].left = thoughts->philosophers[i - 1].right;
	}
	else
		thoughts->philosophers[0].left = NULL;
	return (0);
}

//later: remove
void	init_log(t_deep *thoughts)
{
	// thoughts->fd = open("philosophers.fd\n", O_CREAT | O_TRUNC | O_RDWR, 0644);
	// dup2(thoughts->fd, STDOUT_FILENO);
	thoughts->log = ft_calloc(thoughts->variables[NB_PHILOS] * 100, sizeof(t_log));
	thoughts->log->max = thoughts->variables[NB_PHILOS] * 100;
	pthread_mutex_init(&thoughts->writers_block, NULL);
	thoughts->log->msgs[FORK] = FORK_MSG;
	thoughts->log->msgs[FORK2] = FORK2_MSG;
	thoughts->log->msgs[EAT] = EATING_MSG;
	thoughts->log->msgs[SLEEP] = SLEEPING_MSG;
	thoughts->log->msgs[THINK] = THINKING_MSG;
	thoughts->log->msgs[DIE] = DEATH_MSG;
	// thoughts->log->msgs[END] = FORK_MSG;
	//todo: experiment with logsize
	thoughts->log->queue = ft_calloc(1000, sizeof(t_msg));
	
}

bool	legal_input(int ac, char **av)
{
	int	i;
	int	x;

	i = 0;
	if (ac < 4)
		return (false);
	while (av[++i])
	{
		x = -1;
		while (av[i][++x])
		{
			if ((av[i][x] < '0' || av[i][x] > '9'))
				return (false);
		}
	}
	return (true);
}

int		init_deepthought(int ac, char **av, t_deep *thoughts)
{
	if (!legal_input(ac, av))
	{
		printf ("Bad arguments\n");
		exit (0);
		
	}
	if (ac >= 5)
	{
		thoughts->variables[NB_PHILOS] = ft_atoi(av[NB_PHILOS + 1]);
		// thoughts->variables[TT_DIE]    = 1000 * ft_atoi(av[TT_DIE + 1]);
		// thoughts->variables[TT_EAT]    = 1000 * ft_atoi(av[TT_EAT + 1]);
		// thoughts->variables[TT_SLEEP]  = 1000 * ft_atoi(av[TT_SLEEP + 1]);
			thoughts->variables[TT_DIE]    = ft_atoi(av[TT_DIE + 1]);
			thoughts->variables[TT_EAT]    = ft_atoi(av[TT_EAT + 1]);
			thoughts->variables[TT_SLEEP]  = ft_atoi(av[TT_SLEEP + 1]);
		thoughts->variables[NB_MEALS]  = 1;
		if (ac == 6)
			thoughts->variables[NB_MEALS]  = ft_atoi(av[NB_MEALS + 1]);
		else
			thoughts->variables[NB_MEALS] = -1;
		if (ac > 6)
			printf ("too many args bruh\n");
	}
	// else
	// {
	// 	thoughts->variables[NB_PHILOS] = DEFAULT_NB_PHILOS;
	// 	thoughts->variables[TT_DIE]    = DEFAULT_TT_DIE;
	// 	thoughts->variables[TT_EAT]    = DEFAULT_TT_EAT;
	// 	thoughts->variables[TT_SLEEP]  = DEFAULT_TT_SLEEP;
	// 	thoughts->variables[NB_MEALS]  = DEFAULT_NB_MEALS;
	// }
	if (thoughts->variables[NB_PHILOS] < 0 || thoughts->variables[TT_DIE] < 0 || thoughts->variables[TT_EAT] < 0 || 
		thoughts->variables[TT_SLEEP] < 0)
	{
		printf ("bad input\n");
		exit (0);
	}

	// 	printf ("philos count  = [%i]\n", thoughts->variables[NB_PHILOS]);
	// 	printf ("time to die   = [%i]\n", thoughts->variables[TT_DIE]);
	// 	printf ("time to eat   = [%i]\n", thoughts->variables[TT_EAT]);
	// 	printf ("time to sleep = [%i]\n", thoughts->variables[TT_SLEEP]);
	// 	printf ("meals count   = [%i]\n", thoughts->variables[NB_MEALS]);

	init_philo(thoughts);
	return (0);
}

void	init_philosophers(int ac, char **av, t_deep *thoughts)
{
	init_deepthought(ac, av, thoughts); //todo: setup failstate
	init_log(thoughts);
	pthread_mutex_init(&thoughts->sync, NULL);
	// usleep (1);
	pthread_mutex_lock(&thoughts->sync);
	// printf ("Initialising simulation\n");	
}