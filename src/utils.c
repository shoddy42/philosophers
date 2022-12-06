/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 06:53:35 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 10:42:55 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	//to get time in micro seconds
	return (time.tv_sec * 1000000 + time.tv_usec);
	//to get time in mili seconds
	// return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

/**
 * @brief Allocates a block of memory of count * size, and sets it all to NULL 
 * 
 * @param count Number of elements to allocate for.
 * @param size 	Size of the elements.
 * @return void* of count * size on success. NULL on fail.
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*mem;
	size_t	i;

	mem = malloc(count * size);
	if (!mem)
		return (NULL);
	i = -1;
	while (++i < (count * size))
		((char *)mem)[i] = '\0';
	return (mem);
}

/**
 * @brief Turns a string into an integer.
 * 
 * @param nb the string to be converted.
 * @return @b [int] 
 */
int	ft_atoi(const char *nb)
{
	int				i;
	unsigned long	r;
	int				sign;

	i = 0;
	r = 0;
	sign = 1;
	while (nb[i] == ' ' || nb[i] == '\t' || nb[i] == '\v'
		|| nb[i] == '\f' || nb[i] == '\r' || nb[i] == '\n')
		i++;
	if (nb[i] == '-')
		sign = -1;
	if (nb[i] == '-' || nb[i] == '+')
		i++;
	while (nb[i] >= '0' && nb[i] <= '9')
	{
		r = (r * 10) + (nb[i] - '0');
		i++;
	}
	return ((int)(r * sign));
}

void	get_forks(t_phil *philo, t_deep *thoughts)
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
