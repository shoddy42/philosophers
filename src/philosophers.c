/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: root <root@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/13 01:08:00 by root          #+#    #+#                 */
/*   Updated: 2022/11/13 03:57:59 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void    philosophers(int philo_count, int death, int eat, int sleep, int times)
{
	int	*philosophers;
	int	i;

	philosophers = malloc(philo_count * sizeof(int));
	i = 0;
	if (!philosophers)
		return ;
	while (i < philo_count)
	{
		philosophers[i] = pthread_create(NULL, NULL, NULL, NULL);
		i++;
	}
	
}


int main(void)
{



	return (0);
}