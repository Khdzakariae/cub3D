/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achahid- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 08:46:01 by achahid-          #+#    #+#             */
/*   Updated: 2023/12/01 08:46:03 by achahid-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif /* BUFFER_SIZE */
# include <stdlib.h>
# include <unistd.h>
# include "../lib/libft/libft.h"

int     get_next_line(int fd, char **line);

#endif /* GET_NEXT_LINE_H */
