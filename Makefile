NAME = miniRT

# since i wrote a nice tester that i am happy with
# i want to use it for this project too!
# but it needs a library or c files to compile the test with
# so here is it!
TEST_BINDER_NAME = TestBinder.a

OS = $(shell uname -s)
ifeq ($(OS), Darwin)
	MINI_LIBX_DIR = minilibx-mac
else ifeq ($(OS), Linux)
	MINI_LIBX_DIR = minilibx-linux
else
	$(error Unknown OS: $(OS))
endif

SRC_DIR = src/
OBJ_DIR = obj/
DEBUG_DIR = obj_debug/
INCLUDE_DIRS = include libft/include $(MINI_LIBX_DIR)
INCLUDE = -Iinclude -Ilibft/include -I$(MINI_LIBX_DIR)
LDFLAGS = -L$(shell pwd)/$(MINI_LIBX_DIR)/ -lmlx -Llibft -lft -lm -lpthread

FOREIGN_TARGETS = libft/libft.a
MAKE_FOREIGN_TARGETS = libft/libft $(MINI_LIBX_DIR)/libmlx

ifeq ($(OS), Darwin)
FOREIGN_TARGETS += $(MINI_LIBX_DIR)/libmlx.dylib
else
FOREIGN_TARGETS += $(MINI_LIBX_DIR)/libmlx.a
endif

FLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror
#FLAGS = -DBUFFER_SIZE=128 # warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning

FLAGS += -D OS_$(OS)

ifeq ($(OS), Darwin)
	#LDFLAGS += -framework OpenGL -framework AppKit
	#LDFLAGS += -DYLD_INSERT_LIBRARIES=minilibx-mac/libmlx.dylib
else ifeq ($(OS), Linux)
	LDFLAGS += -lXext -lX11 -lbsd
else
	$(error Unknown OS: $(OS))
endif

ifdef DEBUG
	FLAGS += -g -fsanitize=address
	USE_OBJ_DIR = $(DEBUG_DIR)
	OTHER_OBJ_DIR = $(OBJ_DIR)
else
	FLAGS += -O2
	USE_OBJ_DIR = $(OBJ_DIR)
	OTHER_OBJ_DIR = $(DEBUG_DIR)
endif

ifdef BONUS
FLAGS += -D BONUS
endif

#SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)
#HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name *.h)

SOURCE_FILES = src/general_utils/bmp_utils/save_image.c \
src/general_utils/hilbert_curve/hilbert_curve.c \
src/general_utils/hilbert_curve/hilbert_randomizer.c \
src/general_utils/hilbert_curve/hilbert_randomizer_init.c \
src/general_utils/math_utils/color_math.c \
src/general_utils/math_utils/to_hdr.c \
src/hooks/hooks_frame.c \
src/hooks/hooks_input.c \
src/hooks/hooks_mlx_loop.c \
src/hooks/hooks_mlx_loop2.c \
src/hooks/hooks_mouse_bonus.c \
src/hooks/hooks_mouse.c \
src/init/ft_printf_init.c \
src/init/init_image.c \
src/init/main/main.c \
src/init/main/main_2.c \
src/init/main/mlx_destroy_display.c \
src/init/mlx_init.c \
src/init/mlx_un_init.c \
src/init/renderer_init.c \
src/init/renderer_un_init.c \
src/material/material_additive_bonus.c \
src/material/material_checkerboard_bonus.c \
src/material/material_diffuse/add_diffuse_lighting.c \
src/material/material_diffuse/material_diffuse.c \
src/material/material_emissive_bonus.c \
src/material/material_free.c \
src/material/material_mix_bonus.c \
src/material/material_perfect_mirror_bonus.c \
src/material/material_specular/material_specular_bonus.c \
src/material/material_specular/material_specular_init_bonus.c \
src/parsing/parse_args.c \
src/parsing/parse_line/parse_ambiant.c \
src/parsing/parse_line/parse_anti_aliasing_bonus.c \
src/parsing/parse_line/parse_camera.c \
src/parsing/parse_line/parse_cube_bonus.c \
src/parsing/parse_line/parse_cylinder.c \
src/parsing/parse_line/parse_directional_light_bonus.c \
src/parsing/parse_line/parse_gi_bonus.c \
src/parsing/parse_line/parse_light.c \
src/parsing/parse_line/parse_noise_reduction_bonus.c \
src/parsing/parse_line/parse_obj/insert_obj_into_scene_bonus.c \
src/parsing/parse_line/parse_obj/scene_parse_obj_bonus.c \
src/parsing/parse_line/parse_plane.c \
src/parsing/parse_line/parse_resulution.c \
src/parsing/parse_line/parse_sphere.c \
src/parsing/parse_line/parse_square.c \
src/parsing/parse_line/parse_triangle.c \
src/parsing/parse_material/create_shared_ptr_from_material.c \
src/parsing/parse_material/parse_material_1_bonus.c \
src/parsing/parse_material/parse_material_2_bonus.c \
src/parsing/parse_material/read_material.c \
src/parsing/parse_obj/parse_obj_bonus.c \
src/parsing/parse_obj/parse_obj_file_bonus.c \
src/parsing/parse_obj/parse_obj_line_bonus.c \
src/parsing/parse_obj/parse_obj_line_face_bonus.c \
src/parsing/parse_scene/parse_line.c \
src/parsing/parse_scene/parse_object.c \
src/parsing/parse_scene/parse_scene.c \
src/parsing/parse_scene/parse_scene_file.c \
src/parsing/parse_scene/parse_scene_free.c \
src/parsing/parse_utils.c \
src/raytracing/bvh/build_bvh.c \
src/raytracing/bvh/build_bvh_raw/build_bvh_raw.c \
src/raytracing/bvh/build_bvh_raw/build_bvh_utils.c \
src/raytracing/bvh/build_bvh_raw/bvh_from_objects.c \
src/raytracing/bvh/bvh.c \
src/raytracing/bvh/split_objects.c \
src/raytracing/cast_ray.c \
src/raytracing/intersect_functions/ray_intersects_aabb.c \
src/raytracing/intersect_functions/ray_intersects_cube/ray_intersects_cube_bonus.c \
src/raytracing/intersect_functions/ray_intersects_cube/stupid_norm_bonus.c \
src/raytracing/intersect_functions/ray_intersects_cylinder/ray_intersects_cylinder.c \
src/raytracing/intersect_functions/ray_intersects_cylinder/stupid_norm.c \
src/raytracing/intersect_functions/ray_intersects_plane.c \
src/raytracing/intersect_functions/ray_intersects_sphere.c \
src/raytracing/intersect_functions/ray_intersects_square/ray_intersects_square.c \
src/raytracing/intersect_functions/ray_intersects_triangle/ray_intersects_triangle.c \
src/raytracing/pix_to_ray/pix_to_ray.c \
src/raytracing/trace_color.c \
src/render_loop/edge_detect_bonus.c \
src/render_loop/is_frame.c \
src/render_loop/new_pixel_render_thread_bonus.c \
src/render_loop/noise_detection_bonus.c \
src/render_loop/render_next_pixels/render_next_pixels.c \
src/render_loop/render_next_pixels/render_next_pixels_2.c \
src/render_loop/render_pixel.c \
src/render_loop/update_image.c

HEADER_FILES = include/mini_rt_args.h \
include/mini_rt_bmp.h \
include/mini_rt_bvh.h \
include/mini_rt_color.h \
include/mini_rt_hilbert_randomizer.h \
include/mini_rt_hooks.h \
include/mini_rt_image.h \
include/mini_rt_init.h \
include/mini_rt_material.h \
include/mini_rt_material_data.h \
include/mini_rt_mlx.h \
include/mini_rt_object.h \
include/mini_rt_object_data.h \
include/mini_rt_parse_obj.h \
include/mini_rt_parse_scene_line.h \
include/mini_rt_parse_utils.h \
include/mini_rt_pixel.h \
include/mini_rt_ray_intersects.h \
include/mini_rt_raycast.h \
include/mini_rt_render_pixel.h \
include/mini_rt_renderer.h \
include/mini_rt_scene.h

#filter out the mac/linux/bonus/nonbonus files for what case we need now
BONUS_FILES = $(filter %_bonus.c,$(SOURCE_FILES))

MAC_FILES = $(filter %_mac.c,$(SOURCE_FILES)) $(filter %_mac_bonus.c,$(SOURCE_FILES))
LINUX_FILES = $(filter %_linux.c,$(SOURCE_FILES)) $(filter %_linux_bonus.c,$(SOURCE_FILES))

ifdef BONUS
	NON_BONUS = $(patsubst %_bonus.c,%.c,$(BONUS_FILES))
	#BONUS_FILTERED_FILES = $(filter-out $(NON_BONUS),$(SOURCE_FILES))
	BONUS_FILTERED_FILES = $(SOURCE_FILES)	# meh.
else
	#BONUS_FILTERED_FILES = $(filter-out $(BONUS_FILES),$(SOURCE_FILES))
	BONUS_FILTERED_FILES = $(SOURCE_FILES)	# meh.
endif

ifdef LINUX
	FILTERED_SOURCE_FILES = $(filter-out $(MAC_FILES),$(BONUS_FILTERED_FILES))
else
	FILTERED_SOURCE_FILES = $(filter-out $(LINUX_FILES),$(BONUS_FILTERED_FILES))
endif

OBJECTS = $(FILTERED_SOURCE_FILES:$(SRC_DIR)%.c=$(USE_OBJ_DIR)%.o)

.PHONY: all
all: $(NAME)

.PHONY: debug
debug:
	$(MAKE) DEBUG=1

.PHONY: bonus
bonus:
	$(MAKE) BONUS=1

_ = $(shell make --no-print-directory -C libft)
_ = $(shell make --no-print-directory -C $(MINI_LIBX_DIR))

$(NAME): $(OBJECTS) $(FOREIGN_TARGETS) | $(OBJ_DIR) $(DEBUG_DIR)
ifeq ($(OS), Darwin)
	cp $(MINI_LIBX_DIR)/libmlx.dylib libmlx.dylib
endif
	$(CC) $(FLAGS) -o $(NAME) $(OBJECTS) $(LDFLAGS)
	# mark non-debug or debug dirty, after making miniRT
	touch -c $$(find $(OTHER_OBJ_DIR))

$(TEST_BINDER_NAME): $(OBJECTS)
	rm -f $(TEST_BINDER_NAME)
	ar -rc $(TEST_BINDER_NAME) $(filter-out %main.o,$(OBJECTS))

$(OBJ_DIR) $(DEBUG_DIR) $(SRC_DIR):
	mkdir $@

$(OBJECTS): $(USE_OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES) \
	| $(SRC_DIR) $(USE_OBJ_DIR)
	mkdir -p $(shell dirname $@)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

$(FOREIGN_TARGETS):
	make --no-print-directory -C $(shell dirname $@)

# cleanup targets
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(DEBUG_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_BINDER_NAME)

.PHONY: re
re:
	$(MAKE) fclean
	$(MAKE) all