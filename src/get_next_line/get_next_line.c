/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 14:24:54 by jsimonis      #+#    #+#                 */
/*   Updated: 2020/12/03 15:07:29 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

/*
**	The un-returned data goes from buff_inx to buff_end
**	To re-use the space before buff_inx we move buff_inx to 0
**	so that we "free" the data that we have already returned
*/

static void			collapse_buffer(t_fd_data *data)
{
	if (data->buff_inx == 0)
		return ;
	mem_cpy(data->buffer, &data->buffer[data->buff_inx],
		data->buff_end - data->buff_inx);
	data->buff_end -= data->buff_inx;
	data->buff_inx = 0;
}

/*
**	Reads from the file fd and appends it to buffer
*/

static t_gnl_out	read_next_part(int fd, t_fd_data *data)
{
	ssize_t read_chars;

	if (data->buff_end + BUFFER_SIZE >= data->buff_size)
	{
		collapse_buffer(data);
		if (data->buff_end + BUFFER_SIZE >= data->buff_size)
			if (!increase_buffer_size(data))
				return (gnl_err);
	}
	read_chars = read(fd, &data->buffer[data->buff_end], BUFFER_SIZE);
	if (read_chars == -1)
		return (gnl_err);
	data->buff_end += read_chars;
	if (read_chars != BUFFER_SIZE)
		return (gnl_eof);
	return (gnl_success);
}

static t_gnl_out	buffer_to_line(t_fd_data *data, char **line, size_t length)
{
	*line = malloc(length + 1);
	if (*line == NULL)
		return (gnl_err);
	mem_cpy(*line, &data->buffer[data->buff_inx], length);
	(*line)[length] = '\0';
	if (length + data->buff_inx >= data->buff_end)
		return (gnl_eof);
	data->buff_inx += length + 1;
	return (gnl_success);
}

static t_gnl_out	read_line(int fd, t_fd_data *data, char **line)
{
	size_t len;

	len = 0;
	while (true)
	{
		if (data->buff_inx + len >= data->buff_end)
			if (read_next_part(fd, data) == gnl_err)
				return (gnl_err);
		if (data->buff_inx + len >= data->buff_end
			|| data->buffer[data->buff_inx + len] == '\n')
			return (buffer_to_line(data, line, len));
		len++;
	}
}

int					get_next_line(int fd, char **line)
{
	t_fd_data	*fd_data;
	t_gnl_out	out;

	fd_data = get_gnl_data(fd);
	if (fd_data == NULL)
		return (gnl_err);
	*line = NULL;
	if (fd_data->buffer == NULL)
	{
		fd_data->buffer = malloc(BUFFER_SIZE);
		if (fd_data->buffer == NULL)
			return (gnl_err);
		fd_data->buff_inx = 0;
		fd_data->buff_end = 0;
		fd_data->buff_size = BUFFER_SIZE;
	}
	out = read_line(fd, fd_data, line);
	if (out == gnl_eof || out == gnl_err)
		clear_gnl_data(fd);
	return (out);
}
