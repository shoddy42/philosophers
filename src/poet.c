/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   poet.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/02 06:09:09 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 16:14:48 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	add_queue(unsigned long time, t_msg_type type, int id, t_deep *thoughts)
{
	t_log	*log;

	log = thoughts->log;
	pthread_mutex_lock(&thoughts->writers_block);
	if (log->size < 0)
	{
		pthread_mutex_unlock(&thoughts->writers_block);
		return ;
	}
	log->queue[log->size].type = type;
	log->queue[log->size].time = time;
	log->queue[log->size].id = id;
	log->size++;
	if (log->size > log->max)
		printf("LOG SOMEHOW FULL!!!\n");
	pthread_mutex_unlock(&thoughts->writers_block);
}

static void	print_queue(t_deep *thoughts)
{
	t_log	*log;
	int		i;

	i = -1;
	log = thoughts->log;
	while (++i < log->size)
	{
		printf(FORMAT_MSG , log->queue[i].time, log->queue[i].id, log->msgs[log->queue[i].type]);
		if (log->queue[i].type == DIE)
		{
			log->size = -2000;
			// printf ("Last meal: [%li] diff: [%li]\n", thoughts->philosophers[log->queue[i].id].last_supper - thoughts->epoch, ((get_time() - thoughts->epoch)) - (thoughts->philosophers[log->queue[i].id].last_supper - thoughts->epoch));
			return ;
		}
	}
	log->size -= i;
}

void	*shakespeare(void *param)
{
	t_deep	*thoughts;
	t_log	*log;
	bool	existance;

	thoughts = param;
	log = thoughts->log;
	existance = true;
	while (existance)
	{
		existance = confirm_reality(thoughts);
		usleep (500);
		pthread_mutex_lock(&thoughts->writers_block);
		print_queue(thoughts);
		if (log->size > 0)
			existance = true;
		pthread_mutex_unlock(&thoughts->writers_block);
	}
	return (NULL);
}