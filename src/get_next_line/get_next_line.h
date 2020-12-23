/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 14:25:36 by jsimonis      #+#    #+#                 */
/*   Updated: 2020/11/26 13:15:30 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  warning no BUFFER_SIZE specified, defaulting to 32
#  define BUFFER_SIZE 32
# elif BUFFER_SIZE <= 0
#  warning BUFFER_SIZE <= 0, defaulting to 32
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

typedef	struct	s_fd_data
{
	char	*buffer;
	size_t	buff_inx;
	size_t	buff_end;
	size_t	buff_size;
}				t_fd_data;

typedef enum	e_gnl_out
{
	gnl_success = 1,
	gnl_eof = 0,
	gnl_err = -1
}				t_gnl_out;

typedef	struct	s_gnl_ll
{
	int				fd;
	t_fd_data		data;
	struct s_gnl_ll	*next;
	struct s_gnl_ll	*previous;
}				t_gnl_ll;

int				get_next_line(int fd, char **line);
bool			clear_gnl_data(int fd);
t_fd_data		*get_gnl_data(int fd);
void			mem_cpy(char *dst, char *src, size_t len);
bool			increase_buffer_size(t_fd_data *data);

#endif
