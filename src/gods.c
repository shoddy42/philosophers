/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gods.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/02 04:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/08 13:14:12 by wkonings      ########   odam.nl         */
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
	// if ((((time - thoughts->epoch) - (mortal->last_supper - thoughts->epoch))) / 1000 <= (unsigned long)thoughts->variables[TT_DIE] / 1000)
	if (((time - thoughts->epoch) - (mortal->last_supper - thoughts->epoch)) <= (unsigned long)thoughts->variables[TT_DIE])
	{
		return (false);
	}
	printf("\1\33[38;5;196m%8lu %i has THE DED\n", get_time() - thoughts->epoch, mortal->id);
	// pthread_mutex_lock(&thoughts->writers_block);
	// thoughts->log->print_size = -2000;
	// thoughts->log->queue_size = -2000;
	// pthread_mutex_unlock(&thoughts->writers_block);
	add_queue(get_time() - thoughts->epoch, DIE, mortal->id, thoughts);
	// end_universe(thoughts);
	// printf ("%li %i dead. last? %li\n", (time - thoughts->epoch), mortal->id, mortal->last_supper - thoughts->epoch);
	// printf ("%li <= %li\n", mortal->last_supper - thoughts->epoch, (unsigned long)thoughts->variables[TT_DIE]);
	// printf ("?: %li\n", (time - thoughts->epoch) - (mortal->last_supper - thoughts->epoch));
	// printf ("\1\33[38;5;182m %li %i dead. last? %li\n\2\3", (time - mortal->last_supper), mortal->id, mortal->last_supper - thoughts->epoch);
	pthread_mutex_unlock(&mortal->soul);
	// pthread_mutex_unlock()
	end_universe(thoughts);
	// printf ("Hello!?\n");
	return (true);
}

bool	observe(t_deep *thoughts)
{
	t_phil	*mortal;
	int		i;

	i = -1;
	thoughts->satisfied = 0;
	// printf ("THE WATCH HAS BEGUN.\n");
	while (++i < thoughts->variables[NB_PHILOS])
	{
		mortal = thoughts->philosophers + i;
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
	// ponder_death(NULL, thoughts, TT_DIE - 1);
	smart_sleep(thoughts->variables[TT_DIE] - 10);
	while (observe(thoughts))
		usleep (500);
	return (NULL);
}
