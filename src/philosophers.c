/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/01 04:42:52 by wkonings      ########   odam.nl         */
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

long	time_(struct timeval time, t_deep *thoughts)
{
	long	ret;


	ret = ((time.tv_sec * 1000 + time.tv_usec / 1000)) - ((thoughts->epoch.tv_sec * 1000 + thoughts->epoch.tv_usec / 1000));
	// printf ("time: [%li]\n", ret);
	return (ret);	
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ponder_death(t_phil *philo, t_deep *thoughts, int time)
{
	struct timeval delta;
	struct timeval start;

	gettimeofday(&start, NULL);
	gettimeofday(&delta, NULL);
	while (time_(delta, thoughts) - time_(start, thoughts) < time)
	{
		gettimeofday(&delta, NULL);
		// printf ("time delta [%li], death time [%i]\n", (time_(delta, thoughts) - time_(philo->last_supper, thoughts)), thoughts->variables[TT_DIE]);
		if (time_(delta, thoughts) - time_(philo->last_supper, thoughts) > thoughts->variables[TT_DIE])
		{
			printf ("%li DEATH HAS OCCURED! %i has died. Last sup [%li]. Death [%li] diff [%li]\n", time_(delta, thoughts), philo->id, time_(philo->last_supper, thoughts), time_(delta, thoughts), time_(delta, thoughts) - time_(philo->last_supper, thoughts));
			exit (0);
		}
		// printf ("time [%li]\n", time_(delta, thoughts) - time_(start, thoughts));
		usleep (50);
	}
}

void *yes(void *param)
{
	t_phil			*philo;
	struct timeval	t;
	struct timeval	start;


	philo = param;
	// if (philo->id != 1)
	// 	return (NULL);
	while (philo->meals > 0)
	{
		//GRAB FORKS.

		gettimeofday(&t, NULL);
		printf ("%li %i is thinking\n", time_(t, philo->thoughts), philo->id);
		//when philosopher X is eating he takes fork X and fork X - 1
		pthread_mutex_lock(philo->left);
		gettimeofday(&t, NULL);
		printf ("%li %i has taken a fork\n", time_(t, philo->thoughts), philo->id);

		pthread_mutex_lock(philo->right);
		gettimeofday(&t, NULL);
		printf ("%li %i has taken a fork\n", time_(t, philo->thoughts), philo->id);

		//EAT WHILE PONDERING DEATH.
		gettimeofday(&t, NULL);
		printf ("%li %i is eating for [%i], last meal at [%li]. Forks [%i][%i] are locked.\n", time_(t, philo->thoughts), philo->id, philo->thoughts->variables[TT_EAT], time_(philo->last_supper, philo->thoughts), philo->id, philo->id - 1 < 1 ? philo->thoughts->variables[NB_PHILOS] : philo->id - 1);
		gettimeofday(&philo->last_supper, NULL);

			ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_EAT]);

		//finished eating. continue to ponder.
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		// printf ("%li %i finished eating, %i meals remain\n", time_(t, philo->thoughts), philo->id, philo->meals);
		gettimeofday(&t, NULL);
		printf ("%li %i is sleeping for [%i]\n", time_(t, philo->thoughts), philo->id, philo->thoughts->variables[TT_SLEEP]);
		//replace with ponder_death
			// usleep(philo->thoughts->variables[TT_SLEEP]);

		//finished sleeping
		ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_SLEEP]);
		gettimeofday(&t, NULL);
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
		gettimeofday(&time, NULL);
		while (++i < thoughts->variables[NB_PHILOS])
		{
			if (time_(time, thoughts) - time_(thoughts->philosophers[i].last_supper, thoughts) > thoughts->variables[TT_DIE])
			{

				printf ("%li %i DEAD MAN WALKING!\n", time_(time, thoughts), thoughts->philosophers[i].id);			
				exit (0);
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
	//SET DEBUG LEVEL
	thoughts->debuglevel = 5;


	init_deepthought(ac, av, thoughts); //todo: setup failstate
	init_time(thoughts);

	//later: remove
	// init_log(thoughts);
	create_threads(thoughts);
	// watch_threads(thoughts);
	pthread_create(&thoughts->plato, NULL, watch_threads, (void *)thoughts);
	// printf ("TEST HELLO!\n");
	join_threads(thoughts);
	return (0);
}