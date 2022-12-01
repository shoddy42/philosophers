/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/01 04:34:08 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

// cursed zone
# include <fcntl.h> //later: remove
# include <stdio.h> //later: remove
# define DEFAULT_NB_PHILOS 5
# define DEFAULT_TT_DIE 1100
# define DEFAULT_TT_EAT 580
# define DEFAULT_TT_SLEEP 500
# define DEFAULT_NB_MEALS 8





// end of cursed zone 
typedef struct s_deepthought	t_deep;

/** 
 * @brief	Enumerators for clarity when accessing
 *
 * @param	NB_PHILOS The amount of philosophers in the simulation.
 * @param	TT_DIE	The time (in ms?) it takes for a philosopher to die.
 * @param	TT_EAT	The time (in ms?) it takes for a philosopher to eat.
 * @param	TT_SLEEP The time (in ms?) it takes for a philosopher to sleep.
 * @param	NB_MEALS The number of times the philosophers will eat
 * 					 before ending the simulation.
 */
enum e_variables
{
	NB_PHILOS,
	TT_DIE,
	TT_EAT,
	TT_SLEEP,
	NB_MEALS,
};

typedef enum e_state
{
	EATING,
	SLEEPING,
	THINKING,
	DEAD,
	CHILLING,
	THIEVING,
}	t_state;


typedef struct s_philosopher
{
	pthread_t		thread;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	
	struct timeval	last_supper;
	int				meals;
	int				id;
	t_deep			*thoughts;
}	t_phil;

/**
 * @brief	The main program data for philosophers
 * @param	variables[5] Contains variables that affect the simulation,
 * 						 defined by e_variables
 * @param	philosophers Has pointers to all philosophers in the simulation.
 * 
 */
typedef struct s_deepthought
{
	int				variables[5];
	t_phil			*philosophers;
	struct timeval	epoch;
	long			init_time;

	pthread_t		plato;
	pthread_mutex_t	sync;
	bool			satisfied;

	//debug zone
	int	debuglevel;
	int	log;
}	t_deep;

// philosophers.c
long	time_(struct timeval time, t_deep *thoughts);

// utils.c
void	*ft_calloc(size_t count, size_t size);
int		ft_atoi(const char *nb);

// init.c
int		init_deepthought(int ac, char **av, t_deep *thoughts);
void	init_time(t_deep *thoughts);


// REMOVE FILES
void	init_log(t_deep *thoughts);


#endif