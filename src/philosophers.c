/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/08 13:24:42 by wkonings      ########   odam.nl         */
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
	t_deep			*thoughts;
	unsigned long	time;

	thoughts = philo->thoughts;
	// get_forks(philo, thoughts);

	pthread_mutex_lock(philo->right);
	add_queue(get_time() - thoughts->epoch, FORK, philo->id, thoughts);
	pthread_mutex_lock(philo->left);
	add_queue(get_time() - thoughts->epoch, FORK2, philo->id, thoughts);
	pthread_mutex_lock(&philo->soul);
	time = get_time();
	add_queue(time - thoughts->epoch, EAT, philo->id, thoughts);
	philo->last_supper = time;
	pthread_mutex_unlock(&philo->soul);
	
	smart_sleep(philo->thoughts->variables[TT_EAT]);

	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_lock(&philo->soul);
	philo->meals++;
	pthread_mutex_unlock(&philo->soul);
}

void *yes(void *param)
{
	t_phil	*philo;
	t_deep	*thoughts;
	t_state state;
	bool	existance;

	philo = param;

	thoughts = philo->thoughts;
	existance = true;
	state = THINKING;
	pthread_mutex_lock(&thoughts->sync);
	pthread_mutex_unlock(&thoughts->sync);
	philo->last_supper = thoughts->epoch;
	if (philo->id % 2 == 0)
		usleep(200);
	while (existance)
	{
		if (state == THINKING)
			add_queue(get_time() - thoughts->epoch, THINK, philo->id, thoughts);
		else if (state == EATING)
			consume(philo);
		else if (state == SLEEPING)
		{
			add_queue(get_time() - thoughts->epoch, SLEEP, philo->id, thoughts);
			smart_sleep(philo->thoughts->variables[TT_SLEEP]);
		}
		state = (state + 1) % 3;
		existance = confirm_reality(thoughts);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_deep	*thoughts;

	thoughts = ft_calloc(1, sizeof(t_deep));
	if (!thoughts)
		exit (EXIT_FAILURE);

	init_philosophers(ac, av, thoughts);
	create_threads(thoughts);
	thoughts->epoch = get_time();
	pthread_mutex_unlock(&thoughts->sync);
	watch_threads(thoughts);
	join_threads(thoughts);
	destroy_forks(thoughts);
	// printf ("final time?: [%li]\n", (get_time() - thoughts->epoch));
	return (0);
}