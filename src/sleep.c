/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sleep.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/06 10:29:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 10:42:17 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
	// long	mod_time;

	end_time = get_time() + duration;
	// mod_time = end_time % 1000;
	// if (mod_time > 450)
	// 	mod_time = 450;
	// end_time -= mod_time;
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
		usleep((dif_time * 2) / 3);
		// usleep(dif_time / 2);
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