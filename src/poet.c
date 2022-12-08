/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   poet.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/02 06:09:09 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/08 15:03:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	add_queue(unsigned long time, t_msg_type type, int id, t_deep *thoughts)
{
	t_log	*log;

	log = thoughts->log;
	pthread_mutex_lock(&thoughts->writers_block);
	if (log->queue_size < 0)
	{
		pthread_mutex_unlock(&thoughts->writers_block);
		return ;
	}
	log->queue[log->queue_size].type = type;
	log->queue[log->queue_size].time = time;
	log->queue[log->queue_size].id = id;
	log->queue_size++;
	if (log->queue_size > log->max)
		printf("LOG SOMEHOW FULL!!!\n");
	pthread_mutex_unlock(&thoughts->writers_block);
}

//change to print
static void	print_queue(t_deep *thoughts)
{
	t_log	*log;
	int		i;

	i = -1;
	log = thoughts->log;
	while (++i < log->queue_size)
	{
		printf(FORMAT_MSG , log->queue[i].time, log->queue[i].id, log->msgs[log->queue[i].type]);
		if (log->queue[i].type == DIE)
		{
			log->queue_size = -2000;
			log->print_size = -2000;
			printf ("Last meal: [%li] reported: [%li] diff: [%li]\n", thoughts->philos[log->queue[i].id].last_supper - thoughts->epoch, log->queue[i].time, ((get_time() - thoughts->epoch)) - (thoughts->philos[log->queue[i].id].last_supper - thoughts->epoch));
			return ;
		}
	}
	log->queue_size -= i;
}

void	swap_print_queue(t_log *log)
{
	t_msg	*tmp;
	int		tmp_size;

	tmp = log->queue;
	tmp_size = log->queue_size;

	log->queue = log->print;
	log->queue_size = log->print_size;
	
	log->print = tmp;
	log->print_size = tmp_size;
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
		swap_print_queue(log);
		pthread_mutex_unlock(&thoughts->writers_block);
		print_queue(thoughts);
		if (log->print_size > 0 || log->queue_size > 0)
			existance = true;
	}
	return (NULL);
}