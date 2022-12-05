/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/05 11:25:13 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	confirm_reality(t_deep *thoughts)
{
	bool	existance;

	pthread_mutex_lock(&thoughts->sync);
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
	long	mod_time;

	end_time = get_time() + duration;
	mod_time = end_time % 1000;
	if (mod_time > 450)
		mod_time = 450;
	end_time -= mod_time;
	// printf ("end [%li] start[%li] diff[%li] changed[%li]\n", end_time / 1000, get_time() / 1000, (end_time) - (get_time()), mod_time);
	while (true)
	{
		cur_time = get_time();
		if (cur_time >= end_time)
			return ;
		dif_time = end_time - cur_time;
		if (dif_time <= 10000)
		{
			while (get_time() < end_time)
				usleep(200);
			return ;
		}
		// usleep((dif_time * 2) / 3);
		usleep(dif_time / 2);
	}
	
}

// void	smart_sleep_ms(long duration)
// {
// 	long	end_time;
// 	long	cur_time;
// 	long	dif_time;

// 	end_time = get_time() + duration;
// 	while (true)
// 	{
// 		cur_time = get_time();
// 		if (cur_time >= end_time)
// 			return ;
// 		dif_time = end_time - cur_time;
// 		if (dif_time <= 1)
// 		{
// 			while (get_time() < end_time)
// 				usleep(200);
// 			return ;
// 		}
// 		usleep((dif_time * 2 * 1000) / 3);
// 	}
// }

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

		// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch), philo->id);
	// printf ("%li [%i] picked up fork2\n", (get_time() - thoughts->epoch), philo->id);
	// printf ("%li [%i] is eating\n", (get_time() - thoughts->epoch), philo->id);
		// printf ("%li [%i] is thinking\n", (get_time() - thoughts->epoch), philo->id);
	// ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_EAT]);
static void	consume(t_phil *philo)
{
	t_deep	*thoughts;

	thoughts = philo->thoughts;
	// get_forks(philo, thoughts);

	pthread_mutex_lock(philo->right);
	add_queue(get_time() - thoughts->epoch, FORK, philo->id, thoughts);
	pthread_mutex_lock(philo->left);
	add_queue(get_time() - thoughts->epoch, FORK2, philo->id, thoughts);
	pthread_mutex_lock(&philo->soul);
	philo->last_supper = get_time();
	pthread_mutex_unlock(&philo->soul);
	add_queue(get_time() - thoughts->epoch, EAT, philo->id, thoughts);
	
	smart_sleep(philo->thoughts->variables[TT_EAT]);

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
	existance = true;
	pthread_mutex_lock(&thoughts->sync);
	pthread_mutex_unlock(&thoughts->sync);
	pthread_mutex_lock(&philo->soul);
	philo->last_supper = thoughts->epoch;
	pthread_mutex_unlock(&philo->soul);
	if (philo->id % 2 == 0)
		usleep(5);
	//todo: potentially only do one at a time so no extra messages!
	while (existance)
	{
		add_queue(get_time() - thoughts->epoch, THINK, philo->id, thoughts);
		consume(philo);
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
	printf ("Initialising simulation\n");
	create_threads(thoughts);
	ponder_death(NULL, thoughts, 5);
	// while (get_time() % 1000 > 10)
	// 	usleep(1);
	thoughts->epoch = get_time();
	pthread_mutex_unlock(&thoughts->sync);
	watch_threads(thoughts);
	// usleep (1);
	join_threads(thoughts);
	// destroy_forks(thoughts);
	printf ("final time?: [%li]\n", (get_time() - thoughts->epoch));
	return (0);
}