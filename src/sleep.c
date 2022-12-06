/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sleep.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/06 10:29:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/06 15:52:35 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	smart_sleep(unsigned long duration)
{
	unsigned long	end_time;
	unsigned long	cur_time;
	unsigned long	dif_time;

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
		usleep((dif_time * 2) / 3);
		// usleep(dif_time / 2);
	}
}


// void	smart_sleep_ms(unsigned long duration)
// {
// 	unsigned long	end_time;
// 	unsigned long	cur_time;
// 	unsigned long	dif_time;

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