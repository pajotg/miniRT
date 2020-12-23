#include "mini_rt.h"
#include "test_utils.h"
#include <math.h>

#include <stdio.h>
#include <dirent.h>
#include "libft.h"

#define ScenesPath "/home/jasper/Desktop/miniRT/scenes"

int main(int argc, char *argv[])
{
	TEST_START
		DIR *d;
		struct dirent *dir;
		d = opendir(ScenesPath"/valid");
		if (!d)
			tu_warning_message_exit("Failed to open directory "ScenesPath"/valid");
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type != DT_REG)
				continue;
			char* str = ft_strjoin(ScenesPath"/valid/",dir->d_name);
			int fd = open(str, O_RDONLY);

			if (fd == -1)
				tu_warning_message_exit("Failed to open scene file: %s", str);
			t_scene* scene = parse_scene_file(fd);
			if (!scene)
				tu_ko_message_exit("Failed to parse valid scene: %s, reason: %s",str, get_last_error());
			free(str);
			close(fd);
		}
		closedir(d);
	TEST
		DIR *d;
		struct dirent *dir;
		d = opendir(ScenesPath"/invalid");
		if (!d)
			tu_warning_message_exit("Failed to open directory "ScenesPath"/invalid");
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type != DT_REG)
				continue;
			char* str = ft_strjoin(ScenesPath"/invalid/",dir->d_name);
			int fd = open(str, O_RDONLY);

			if (fd == -1)
				tu_warning_message_exit("Failed to open scene file: %s", str);
			//fprintf(stderr,"Start read %s: %i\n", str, tu_malloc_non_null_count() - tu_free_non_null_count());
			t_scene* scene = parse_scene_file(fd);	// First call mallocs once more than it frees..
			//fprintf(stderr,"End read %s: %i\n", str, tu_malloc_non_null_count() - tu_free_non_null_count());
			if (scene)
				tu_ko_message_exit("Failed to detect invalid valid scene: %s",str);
			free(str);
			close(fd);
		}
		closedir(d);

	TEST_END
	return (0);
}