/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/02 13:22:20 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	confirm_reality(t_deep *thoughts)
{
	bool	existance;

	pthread_mutex_lock(&thoughts->sync);
	// if (thoughts->oblivion == true)
	// 	existance = false;
	// else
	// 	existance = true;
	existance = (!thoughts->oblivion);
	pthread_mutex_unlock(&thoughts->sync);
	return (existance);
}

bool	ponder_death(t_phil *philo, t_deep *thoughts, int time)
{
	long delta;
	long start;

	// adjust to miliseconds from microseconds
	// time *= 1000;
	start = get_time();
	delta = get_time() - start;
	// printf ("time to wait: %i\n", time);
	while (delta <= time - 5)
	{
		delta = get_time() - start;
		usleep (5);
	}
	// printf ("time to waited: %li\n", delta);
	// delta = get_time() - start;
	// printf ("wasted time [%li]\n", delta - time);
	return (false);
}

void	smart_sleep(long duration)
{
	long	end_time;
	long	cur_time;
	long	dif_time;

	end_time = get_time() + duration;
	while (true)
	{
		cur_time = get_time();
		if (cur_time >= end_time)
			return ;
		dif_time = end_time - cur_time;
		if (dif_time <= 1)
		{
			while (get_time() < end_time)
				usleep(200);
			return ;
		}
		usleep((dif_time * 2 * 1000) / 3);
	}
}

static void	get_forks(t_phil *philo, t_deep *thoughts)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left);
		add_queue(get_time() - thoughts->epoch, FORK, philo->id, thoughts);
		// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch) / 1000, philo->id);

		pthread_mutex_lock(philo->right);
		add_queue(get_time() - thoughts->epoch, FORK2, philo->id, thoughts);
	}
	else
	{
		pthread_mutex_lock(philo->left);
		add_queue(get_time() - thoughts->epoch, FORK, philo->id, thoughts);
		// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch) / 1000, philo->id);

		pthread_mutex_lock(philo->right);
		add_queue(get_time() - thoughts->epoch, FORK2, philo->id, thoughts);	
	}
	
}

static void	consume(t_phil *philo)
{
	t_deep	*thoughts;

	thoughts = philo->thoughts;
	// gather forks.
	// get_forks(philo, thoughts);
	pthread_mutex_lock(philo->left);
	// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch), philo->id);
	add_queue(get_time() - thoughts->epoch, FORK, philo->id, thoughts);
	pthread_mutex_lock(philo->right);
		// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch), philo->id);
	add_queue(get_time() - thoughts->epoch, FORK2, philo->id, thoughts);
	// update last meal and message.
	pthread_mutex_lock(&philo->soul);
	philo->last_supper = get_time();
	pthread_mutex_unlock(&philo->soul);
	// printf ("%li [%i] is eating\n", (get_time() - thoughts->epoch), philo->id);
	add_queue(get_time() - thoughts->epoch, EAT, philo->id, thoughts);

	// consume.
	// ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_EAT]);
	smart_sleep(philo->thoughts->variables[TT_EAT]);

	//update meals 
	pthread_mutex_lock(&philo->soul);
	philo->meals++;
	pthread_mutex_unlock(&philo->soul);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}


void *yes(void *param)
{
	t_phil	*philo;
	t_deep	*thoughts;
	bool	existance;

	philo = param;
	thoughts = philo->thoughts;
	pthread_mutex_lock(&thoughts->sync);
	existance = true;
	pthread_mutex_unlock(&thoughts->sync);
	pthread_mutex_lock(&philo->soul);
	philo->last_supper = thoughts->epoch;
	pthread_mutex_unlock(&philo->soul);
	if (philo->id % 2 == 0)
		usleep(1);
	//todo: potentially only do one at a time so no extra messages!
	while (existance)
	{
		// printf ("%li [%i] is thinking\n", (get_time() - thoughts->epoch), philo->id);
		add_queue(get_time() - thoughts->epoch, THINK, philo->id, thoughts);
		consume(philo);
		// printf ("%li [%i] is sleeping\n", (get_time() - thoughts->epoch), philo->id);
		add_queue(get_time() - thoughts->epoch, SLEEP, philo->id, thoughts);

		smart_sleep(philo->thoughts->variables[TT_SLEEP]);
		// ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_SLEEP]);
		existance = confirm_reality(thoughts);
	}
	// printf ("THREAD %i EXITED\n", philo->id);
	return(NULL);
}

int main(int ac, char **av)
{
	t_deep	*thoughts;

	thoughts = ft_calloc(1, sizeof(t_deep));
	if (!thoughts)
		exit (EXIT_FAILURE);
	//SET DEBUG LEVEL

	init_deepthought(ac, av, thoughts); //todo: setup failstate
	init_log(thoughts);
	pthread_mutex_init(&thoughts->sync, NULL);
	// usleep (1);
	pthread_mutex_lock(&thoughts->sync);
	// usleep (1);
	create_threads(thoughts);
	ponder_death(NULL, thoughts, 5);
	thoughts->epoch = get_time();
	pthread_mutex_unlock(&thoughts->sync);
	watch_threads(thoughts);
	// usleep (1);
	join_threads(thoughts);
	printf ("final time?: [%li]\n", (get_time() - thoughts->epoch));
	return (0);
}