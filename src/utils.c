/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 06:53:35 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/29 23:49:54 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"


static int	ft_isspace(char n)
{
	if (n == ' ' || n == '\t' || n == '\v'
		|| n == '\f' || n == '\r' || n == '\n')
		return (1);
	return (0);
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
	while (ft_isspace(nb[i]))
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