/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gods.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/02 04:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/08 15:03:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	end_universe(t_deep *thoughts)
{
	pthread_mutex_lock(&thoughts->sync);
	thoughts->oblivion = true;
	pthread_mutex_unlock(&thoughts->sync);
}

bool	thanatos(t_phil *mortal, t_deep *thoughts)
{
	unsigned long	time;

	time = get_time();
	if (((time - thoughts->epoch) - (mortal->last_supper - thoughts->epoch)) <= (unsigned long)thoughts->variables[TT_DIE])
	{
		return (false);
	}
	add_queue(get_time() - thoughts->epoch, DIE, mortal->id, thoughts);
	pthread_mutex_unlock(&mortal->soul);
	end_universe(thoughts);
	return (true);
}

bool	observe(t_deep *thoughts)
{
	t_phil	*mortal;
	int		i;

	i = -1;
	thoughts->satisfied = 0;
	while (++i < thoughts->variables[NB_PHILOS])
	{
		mortal = thoughts->philos + i;
		pthread_mutex_lock(&mortal->soul);
		if (thanatos(mortal, thoughts))
			return (false);
		if (mortal->meals >= thoughts->variables[NB_MEALS] && thoughts->variables[NB_MEALS] != -1)
			thoughts->satisfied++;
		pthread_mutex_unlock(&mortal->soul);
	}
	if (thoughts->satisfied < thoughts->variables[NB_PHILOS])
		return (true);
	// printf ("ALL ARE SATISFIED! %i\n", thoughts->variables[NB_MEALS]);
	end_universe(thoughts);
	return (false);
}

void	*watch_threads(t_deep *thoughts)
{
	smart_sleep(thoughts->variables[TT_DIE] - 10);
	while (observe(thoughts))
		usleep (500);
	return (NULL);
}
