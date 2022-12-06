/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 10:53:55 by wkonings      ########   odam.nl         */
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
	philo->goal_time = philo->last_supper + thoughts->variables[TT_EAT] + thoughts->variables[TT_SLEEP];
	// printf ("Time diff [%li]\n", philo->goal_time - philo->last_supper);
	pthread_mutex_unlock(&philo->soul);
	add_queue(get_time() - thoughts->epoch, EAT, philo->id, thoughts);
	
	smart_sleep(philo->thoughts->variables[TT_EAT] - (philo->lost_time / 2));

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
		smart_sleep(philo->thoughts->variables[TT_SLEEP] - (philo->lost_time / 2));
		// ponder_death(philo, philo->thoughts, philo->thoughts->variables[TT_SLEEP]);
		existance = confirm_reality(thoughts);
		pthread_mutex_lock(&philo->soul);
		philo->lost_time = get_time() - philo->goal_time;
		pthread_mutex_unlock(&philo->soul);
		// printf ("LOST TIME: [%li]\n", get_time() - philo->goal_time);
	}
	// printf ("THREAD %i EXITED\n", philo->id);
	return(NULL);
}

int	test(t_deep *thoughts)
{
	int i;
	i = -1;
	thoughts->epoch = get_time();
	while (++i < 1000)
	{
		// pthread_mutex_lock(thoughts->philosophers[0].right);
		// pthread_mutex_unlock(thoughts->philosophers[0].right);
		printf ("time[%i] = [%li]\n", i, get_time() - thoughts->epoch);
		// printf ("testtesttesttest\n");
	}
	printf ("time[%i] = [%li]\n", i, get_time() - thoughts->epoch);
	return (0);
}

int main(int ac, char **av)
{
	t_deep	*thoughts;

	thoughts = ft_calloc(1, sizeof(t_deep));
	if (!thoughts)
		exit (EXIT_FAILURE);

	init_philosophers(ac, av, thoughts);
	// return (test(thoughts)); //for testing timeloss in function calls.
	
	create_threads(thoughts);
	ponder_death(NULL, thoughts, 5);
	while (get_time() % 1000 > 10)
		usleep(1);
	thoughts->epoch = get_time();
	pthread_mutex_unlock(&thoughts->sync);
	watch_threads(thoughts);
	// usleep (1);
	join_threads(thoughts);
	destroy_forks(thoughts);
	printf ("final time?: [%li]\n", (get_time() - thoughts->epoch));
	return (0);
}