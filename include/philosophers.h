/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 09:12:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/05 10:33:42 by wkonings      ########   odam.nl         */
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

# define FORMAT_MSG "%8li %i %s\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
// # define FORMAT_MSG "%8li %i %s for [%i]\n"
# define FORK_MSG "\1\33[38;5;117mhas taken fork 1\2\1\33[0m\2\3"
# define FORK2_MSG "\1\33[38;5;117mhas taken fork 2\2\1\33[0m\2\3"
# define EATING_MSG "\1\33[38;5;198mis eating\2\1\33[0m\2\3"
# define SLEEPING_MSG "\1\33[38;5;135mis sleeping\2\1\33[0m\2\3"
# define THINKING_MSG "\1\33[38;5;27mis thinking\2\1\33[0m\2\3"
// # define DEATH_MSG "has died"
# define DEATH_MSG "\1\33[38;5;196mhas died\2\3"



// end of cursed zone
typedef struct s_deepthought	t_deep;
typedef pthread_mutex_t			t_mutex;

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

typedef enum e_message_types
{
	FORK,
	FORK2,
	EAT,
	SLEEP,
	THINK,
	DIE,
	END
} t_msg_type;

typedef struct s_message
{
	t_msg_type	type;
	long		time;
	int			id;
}	t_msg;

typedef struct s_poetspen
{
	const char	*msgs[7];
	t_msg		*queue;
	int			size;
	int			max;
}	t_log;

typedef struct s_philosopher
{
	pthread_t	thread;
	t_mutex		*left;
	t_mutex		*right;
	t_mutex		soul;
	
	long		last_supper;
	int			meals;
	int			id;
	t_deep		*thoughts;
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
	int			variables[5];
	t_phil		*philosophers;
	long		epoch;


	pthread_t	shakespeare;
	t_mutex		writers_block;
	t_log		*log;

	t_mutex		sync;

	int			satisfied;
	bool		oblivion;

	//debug zone
	int	fd;
}	t_deep;

// philosophers.c
long	get_time(void);
void 	*yes(void *param);
bool	confirm_reality(t_deep *thoughts);
void	end_universe(t_deep *thoughts);
bool	ponder_death(t_phil *philo, t_deep *thoughts, int time);
bool	confirm_reality(t_deep *thoughts);

// utils.c
void	*ft_calloc(size_t count, size_t size);
int		ft_atoi(const char *nb);

// init.c
int		init_deepthought(int ac, char **av, t_deep *thoughts);
void	init_time(t_deep *thoughts);
void	create_threads(t_deep *thoughts);
void	join_threads(t_deep *thoughts);
void	destroy_forks(t_deep *thoughts);


// gods.c
void	*watch_threads(t_deep *thoughts);

// poet.c
void	*shakespeare(void *param);
void	add_queue(long time, t_msg_type type, int id, t_deep *thoughts);


// REMOVE FILES
void	init_log(t_deep *thoughts);


#endif