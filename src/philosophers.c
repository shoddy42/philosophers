/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/01 07:53:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/philosophers.h"


// long	time_us(struct timeval time, t_deep *thoughts)
// {
// 	long	ret;


// 	ret = (((time.tv_sec * 1000000) + time.tv_usec)) - (((thoughts->epoch.tv_sec * 1000000) + thoughts->epoch.tv_usec));
// 	// printf ("time: [%li]\n", ret);
// 	return (ret);	
// }

// long	time_(struct timeval time, t_deep *thoughts)
// {
// 	long	ret;


// 	ret = ((time.tv_sec * 1000 + time.tv_usec / 1000)) - ((thoughts->epoch.tv_sec * 1000 + thoughts->epoch.tv_usec / 1000));
// 	// printf ("time: [%li]\n", ret);
// 	return (ret);	
// }

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ponder_death(t_phil *philo, t_deep *thoughts, int time)
{
	long delta;
	long start;

	start = get_time();
	delta = get_time() - start;
	while (delta < time)
	{
		delta = get_time() - start;
		if (delta > thoughts->variables[TT_DIE])
		{
			printf ("%li %i has died\n", delta, philo->id);
			exit (0);
		}
		usleep (50);
	}
}

void *yes(void *param)
{
	t_phil			*philo;
	t_deep			*thoughts;

	philo = param;
	thoughts = philo->thoughts;
	philo->last_supper = thoughts->epoch;
	pthread_mutex_lock(&thoughts->sync);
	pthread_mutex_unlock(&thoughts->sync);

	while (philo->meals > 0)
	{
		//GRAB FORKS.

		printf ("%li is thinking\n", get_time() - thoughts->epoch);
		//when philosopher X is eating he takes fork X and fork X - 1
		pthread_mutex_lock(philo->left);
		printf ("%li %i has taken a fork\n", get_time() - thoughts->epoch, philo->id);

		pthread_mutex_lock(philo->right);
		printf ("%li %i has taken a fork\n", get_time() - thoughts->epoch, philo->id);

		//EAT WHILE PONDERING DEATH.
		printf ("%li %i is eating\n", get_time() - thoughts->epoch, philo->id);
		// printf ("%li %i is eating for [%i], last meal at [%li]. Forks [%i][%i] are locked.\n", get_time(), philo->id, philo->thoughts->variables[TT_EAT], time_(philo->last_supper, philo->thoughts), philo->id, philo->id - 1 < 1 ? philo->thoughts->variables[NB_PHILOS] : philo->id - 1);
		philo->last_supper = get_time();
			ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_EAT]);

		//finished eating. continue to ponder.
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		// printf ("%li %i finished eating, %i meals remain\n", time_(t, philo->thoughts), philo->id, philo->meals);
		printf ("%li %i is sleeping\n", get_time() - thoughts->epoch, philo->id);
		// printf ("%li %i is sleeping for [%i]\n", time_(t, philo->thoughts), philo->id, philo->thoughts->variables[TT_SLEEP]);

		//finished sleeping
		ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_SLEEP]);
		// printf ("[%li] [%i] is finished sleeping\n", time_(t, philo->thoughts), philo->id);
		// usleep(philo->thoughts->variables[TT_SLEEP]);
		philo->meals--;
	}
	return(NULL);
}

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

}

void	*watch_threads(void *param)
{
	t_deep 			*thoughts;
	struct timeval	time;
	int				i;
	thoughts = param;

	// printf ("THE WATCH BEGINS.\n");
	while (!thoughts->satisfied)
	{
		i = -1;
		while (++i < thoughts->variables[NB_PHILOS])
		{
			if (get_time() - thoughts->philosophers[i].last_supper > (long)thoughts->variables[TT_DIE])
			{
				printf ("%li %i DEAD MAN WALKING!\n", get_time() - thoughts->epoch, thoughts->philosophers[i].id);
				exit (0);
				return (NULL);
			}
		}
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_deep	*thoughts;

	thoughts = ft_calloc(1, sizeof(t_deep));
	if (!thoughts)
		exit (EXIT_FAILURE);
	// thoughts->epoch = 56;
	//SET DEBUG LEVEL
	thoughts->debuglevel = 5;


	init_deepthought(ac, av, thoughts); //todo: setup failstate
	//later: remove
	// init_log(thoughts);
	pthread_mutex_lock(&thoughts->sync);
	init_time(thoughts);
	create_threads(thoughts);
	pthread_mutex_unlock(&thoughts->sync);
	watch_threads(thoughts);
	// pthread_create(&thoughts->plato, NULL, watch_threads, (void *)thoughts);
	join_threads(thoughts);
	// exit (0);
	return (0);
}