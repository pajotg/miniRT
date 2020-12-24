/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:19 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/24 18:25:00 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

#include "mini_rt_utils.h"
#include "mini_rt_math_utils.h"
#include <stdbool.h>

typedef struct	s_resolution
{
	int width;
	int height;
}				t_resolution;

typedef struct	s_scene_parse_data
{
	bool has_resolution;
	bool has_ambiant;
}				t_scene_parse_data;

typedef struct	s_scene
{
	t_resolution resolution;
	t_color_hdr ambiant;
	int current_camera_index;
	t_darray cameras;
	t_darray objects;
	t_darray lights;
}				t_scene;

void free_scene(t_scene* scene);
t_scene* parse_scene_file(int fd);

bool skip_char(char* str, int* current, char chr);
void skip_whitespace(char* str, int* current);
bool read_int(char* str, int* current, int* value);
bool read_float(char* str, int* current, float* value);

bool read_vec3(char* str, int* current, t_vec3 *vec3);
bool read_vec3_unit(char* str, int* current, t_vec3 *vec3);
bool read_transform(char* str, int* current, t_transform *transform);
bool read_color(char* str, int* current, bool has_ratio, t_color_hdr *color);

#endif