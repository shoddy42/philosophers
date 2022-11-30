/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/30 02:07:37 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/philosophers.h"

void	*test(void *param)
{
	printf ("thread created\n");

	return (NULL);
}

void	*test2(void *param)
{
	printf ("thread created V2\n");

	return (NULL);
}

void    philosophers(int philo_count, int death, int eat, int sleep, int times)
{
	int error;
	int	i;

	
}

void *yes(void *param)
{
	t_phil	*philo;

	philo = param;
	printf ("Philosopher [%i] is thinking\n", philo->id);

   //when philosopher 5 is eating he takes fork 1 and fork 5
	pthread_mutex_lock(&philo->left);
	pthread_mutex_lock(&philo->right);	
	printf ("Philosopher [%i] is eating\n", philo->id);
	// sleep(3);
	pthread_mutex_unlock(&philo->left);
	pthread_mutex_unlock(&philo->right);
	printf ("Philosopher [%i] finished eating\n", philo->id);
	return(NULL);
}

void	create_threads(t_deep *thoughts)
{
	int	i;
	t_phil *philo;
	pthread_t test;

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
	pthread_t test;

	philo = thoughts->philosophers;

	i = -1;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		pthread_join(philo[i].thread, NULL);		
	}

}

int main(int ac, char **av)
{
	t_deep	*thoughts;
	pthread_t th;
	pthread_t th2;
	int	th_ret;
	int	th_ret2;
	int loop = 0;



	thoughts = ft_calloc(1, sizeof(t_deep));
	if (!thoughts)
		exit (EXIT_FAILURE);
	//SET DEBUG LEVEL
	thoughts->debuglevel = 5;


	init_deepthought(ac, av, thoughts); //setup failstate
	while (++loop < 2)
	{
		create_threads(thoughts);
		join_threads(thoughts);
		
	}
	// th_ret = pthread_create(&th, NULL, test, NULL);
	// th_ret2 = pthread_create(&th2, NULL, test2, NULL);

	// printf ("test\n");
	// usleep(1000);
	// printf ("test\n");
	// printf ("test\n");
	// th_ret = pthread_join(th, NULL);
	// th_ret2 = pthread_join(th2, NULL);
	// th_ret2 = pthread_join(th2, NULL);
	// printf ("test [%i][%i]\n", th_ret, th_ret2);
	return (0);
}