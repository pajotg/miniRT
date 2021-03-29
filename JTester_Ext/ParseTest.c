#include "test_utils.h"
#include <math.h>

#include <stdio.h>
#include <dirent.h>
#include "libft.h"
#include "ft_error.h"

#include "mini_rt_object.h"
#include "mini_rt_mlx.h"
#include "mini_rt_material.h"
#include "mini_rt_pixel.h"
#include "mini_rt_scene.h"
#include "mini_rt_init.h"
#include "mini_rt_hooks.h"
#include "mini_rt_ray_intersects.h"
#include "mini_rt_parse_scene_line.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_bmp.h"
#include "mini_rt_image.h"
#include "mini_rt_renderer.h"
#include "mini_rt_material_data.h"
#include "mini_rt_parse_obj.h"
#include "mini_rt_color.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_args.h"
#include "mini_rt_raycast.h"
#include "mini_rt_object_data.h"
#include "ft_parse_utils.h"

#if LINUX
#define ScenesPath "/home/jasper/Desktop/miniRT/scenes"
#else
#define ScenesPath "/Users/jsimonis/Desktop/miniRT/scenes"
#endif

void get_next(DIR* ValidDir, DIR* InValidDir, struct dirent** dir, bool* ShouldBeValid)
{
	if (*ShouldBeValid)
		*dir = readdir(ValidDir);
	else
		*dir = NULL;
	if (*dir == NULL)
	{
		*ShouldBeValid = false;
		*dir = readdir(InValidDir);
	}
	if (*dir != NULL && (*dir)->d_type != DT_REG)
		get_next(ValidDir, InValidDir, dir, ShouldBeValid);
}

/*
**	This test main is using the internal structure of the test code to give every file its own test result
*/

// TODO: More tests files, and check for NULL mallocs
//	For every line change every parameter a tiny bit

int main(int argc, char *argv[])
{
	tu_test_init(argc, argv);

	// First open both directories
	struct dirent* file;

	DIR* ValidDir;
	ValidDir = opendir(ScenesPath"/valid");
	if (!ValidDir)
		tu_warning_message_exit("Failed to open directory "ScenesPath"/valid");

	DIR* InValidDir;
	InValidDir = opendir(ScenesPath"/invalid");
	if (!InValidDir)
		tu_warning_message_exit("Failed to open directory "ScenesPath"/invalid");

	// Read what test we are on
	int testID;
	int test_number_length = 0;
	if (!read_int(argv[1], &test_number_length, &testID))
		tu_warning_message_exit("Failed to parse test id: %s", argv[1]);

	//fprintf(stderr, "Skipping %i files!\n", testID);
	// Skip [TestID] files, we have already tested them
	bool ShouldBeValid = true;
	for (int i = 0; i < testID; i++)
	{
		get_next(ValidDir, InValidDir, &file, &ShouldBeValid);
		//fprintf(stderr, "	Skipped file: %s\n", file->d_name);
		if (file == NULL)
		{
			tu_warning_message_exit("Failed to call test_stop: %s", argv[1]);
			return 1;
		}
	}

	// Get the test file we are supposed to test
	get_next(ValidDir, InValidDir, &file, &ShouldBeValid);
	char* str = NULL;
	if (!file)
		tu_test_stop();	// Guess we are done here
	else
	{
		// Open the file
		str = ft_strjoin(ShouldBeValid ? ScenesPath"/valid/" : ScenesPath"/invalid/",file->d_name);
		int fd = open(str, O_RDONLY);
		if (fd == -1)
			tu_warning_message_exit("Failed to open scene file: %s", str);

		// Parse the scene
		//fprintf(stderr, "Parsing scene: %s\n",str);
		t_scene* scene = parse_scene_file(str, fd);

		// Check the output
		if (ShouldBeValid && !scene)
			tu_ko_message_exit("Failed to parse valid scene: %s, reason: %s",str, get_last_error());
		else if (!ShouldBeValid && scene)
			tu_ko_message_exit("Failed to detect invalid valid scene: %s",str);


		if (ShouldBeValid)
		{
			for (size_t i = 0; i < scene->objects.count; i++)
			{
				t_object* obj = list_index(&scene->objects, i);
				t_quaternion quat = obj->transform.rotation;
				float sqrmag = quat.r * quat.r + quat.i * quat.i + quat.j * quat.j + quat.k * quat.k;
				if (sqrmag > 1.01 || sqrmag < 0.99)
					tu_ko_message_exit("Failed to set quaternion to valid value for object %lu in scene file: %s", i, str);
				t_vec3 pos = obj->transform.position;
				sqrmag = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
				if (sqrmag > 250*250)
					tu_ko_message_exit("Failed to set position to valid value for object %lu in scene file: %s", i, str);
			}
			for (size_t i = 0; i < scene->cameras.count; i++)
			{
				t_object* obj = list_index(&scene->cameras, i);
				t_quaternion quat = obj->transform.rotation;
				float sqrmag = quat.r * quat.r + quat.i * quat.i + quat.j * quat.j + quat.k * quat.k;
				if (sqrmag > 1.01 || sqrmag < 0.99)
					tu_ko_message_exit("Failed to set quaternion to valid value for camera %lu in scene file: %s", i, str);
				t_vec3 pos = obj->transform.position;
				sqrmag = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
				if (sqrmag > 250*250)
					tu_ko_message_exit("Failed to set position to valid value for camera %lu in scene file: %s", i, str);
			}
		}
		if (scene)
			free_scene(scene);

		close(fd);
	}

	// Close and free all the stuff
	clear_error();	// free the error string in case its malloced
	closedir(ValidDir);
	closedir(InValidDir);

	// Change the leak message to also contain the file we tested
	if (tu_malloc_non_null_count() != tu_free_non_null_count() + 1)	// + 1 because we still have str allocated
	{
		//fprintf(stderr, "Leaks detected for file: %s! mallocs != frees (%i != %i)\n",str, tu_malloc_non_null_count(), tu_free_non_null_count() + 1);
		tu_ko_message_exit("Leaks detected for file: %s! mallocs != frees (%i != %i)",str, tu_malloc_non_null_count(), tu_free_non_null_count() + 1);
	}
	free(str);
	tu_test_finish();
	return 5;
}