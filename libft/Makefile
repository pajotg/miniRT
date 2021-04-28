NAME = libft.a

SRC_DIR = src/
OBJ_DIR = obj/
DEBUG_DIR = obj_debug/
INCLUDE_DIRS = include/
INCLUDE = -Iinclude
FLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror

#SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)
#HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name *.h)

# Wow, i hate this, just let me use find!
SOURCE_FILES = src/ft_is/ft_isdigit.c \
src/ft_is/ft_isprint.c \
src/ft_is/ft_isalnum.c \
src/ft_is/ft_isascii.c \
src/ft_is/ft_ispar.c \
src/ft_is/ft_isspace.c \
src/ft_is/ft_isalpha.c \
src/ft_put/ft_putunicode.c \
src/ft_put/ft_putchar_fd.c \
src/ft_put/ft_putendl_fd.c \
src/ft_put/ft_putchard_fd.c \
src/ft_put/ft_putarr_fd.c \
src/ft_put/ft_putstrd_fd.c \
src/ft_put/ft_putstr_fd.c \
src/ft_put/ft_putnbr_fd.c \
src/ft_mem/ft_calloc.c \
src/ft_mem/ft_memmove.c \
src/ft_mem/ft_memcpy.c \
src/ft_mem/ft_memset.c \
src/ft_mem/ft_memccpy.c \
src/ft_mem/ft_memdup.c \
src/ft_mem/ft_memchr.c \
src/ft_mem/ft_memcmp.c \
src/ft_mem/ft_bzero.c \
src/linked_list/linked_list_clear.c \
src/linked_list/linked_list_map.c \
src/linked_list/linked_list_hascycle.c \
src/linked_list/linked_list_new.c \
src/linked_list/linked_list_size.c \
src/linked_list/linked_list_last.c \
src/linked_list/linked_list_iter.c \
src/linked_list/linked_list_add_front.c \
src/linked_list/linked_list_add_back.c \
src/linked_list/linked_list_delone.c \
src/get_next_line/get_next_line_utils.c \
src/get_next_line/get_next_line.c \
src/manual_reset_event/manual_reset_event.c \
src/ft_printf/default_conversions/def_handle_conversions_fge.c \
src/ft_printf/default_conversions/def_handle_conversions_cspdi.c \
src/ft_printf/default_conversions/def_handle_conversions_u_x_ux_precent_n.c \
src/ft_printf/printf_read_fomat.c \
src/ft_printf/printf_write_util.c \
src/ft_printf/printf_util.c \
src/ft_printf/printf_functions/basic.c \
src/ft_printf/printf_functions/vararg.c \
src/ft_printf/printf_functions/base.c \
src/ft_printf/printf_write_util2.c \
src/ft_printf/write_modes/ft_printf_write_buff.c \
src/ft_printf/write_modes/ft_printf_write_ft.c \
src/ft_printf/printf_handle_conversion.c \
src/ft_init/ft_init_manual_reset_event.c \
src/ft_init/ft_init_open.c \
src/ft_init/ft_init_mutex.c \
src/ft_init/ft_init.c \
src/ft_init/ft_init_mutex_attr.c \
src/ft_init/ft_init_malloc.c \
src/shared_ptr/ft_shared_ptr_access.c \
src/shared_ptr/ft_shared_ptr_init.c \
src/ft_ternary/ft_ternary_floats.c \
src/ft_ternary/ft_ternary_ints.c \
src/ft_ternary/ft_ternary_ptr.c \
src/ft_ternary/ft_ternary_uints.c \
src/ft_parse_utils/ft_parse_utils_skip.c \
src/ft_parse_utils/ft_parse_utils.c \
src/ft_str/ft_split.c \
src/ft_str/ft_strmapi.c \
src/ft_str/ft_strlcpy.c \
src/ft_str/ft_substr.c \
src/ft_str/ft_strlcat.c \
src/ft_str/ft_strrchr.c \
src/ft_str/ft_strjoin.c \
src/ft_str/ft_strjoin_va.c \
src/ft_str/ft_unicode_length.c \
src/ft_str/ft_strnstr.c \
src/ft_str/ft_strchr.c \
src/ft_str/ft_strdup.c \
src/ft_str/ft_strlen.c \
src/ft_str/ft_strncmp.c \
src/ft_str/ft_unicode_tochars.c \
src/ft_str/ft_strcontains.c \
src/ft_str/ft_strtrim.c \
src/list/list_index.c \
src/list/list_access.c \
src/list/list_clear.c \
src/list/list_init.c \
src/list/list_un_init.c \
src/ft_to/ft_toupper.c \
src/ft_to/ft_tolower.c \
src/random/ft_error.c \
src/random/ft_get_time.c \
src/ft_math/ft_math_max.c \
src/ft_math/aabb/aabb_properties.c \
src/ft_math/aabb/aabb_contains.c \
src/ft_math/aabb/aabb.c \
src/ft_math/ft_math_min.c \
src/ft_math/ft_rand.c \
src/ft_math/quaternion/quaternion_constants.c \
src/ft_math/quaternion/quaternion_from.c \
src/ft_math/quaternion/quaternion_math.c \
src/ft_math/vec3d/vec3_constants.c \
src/ft_math/vec3d/vec3_math2.c \
src/ft_math/vec3d/vec3_math3.c \
src/ft_math/vec3d/vec3_math.c \
src/ft_math/vec3d/vec3_constants2.c \
src/ft_math/ft_math_abs.c \
src/ft_ia/ft_itoa_base.c \
src/ft_ia/ft_itoa.c \
src/ft_ia/ft_atoi.c \
src/ft_bmp/read_bmp.c \
src/ft_bmp/write_bmp.c
HEADER_FILES = include/ft_init.h \
include/ft_printf_int.h \
include/ft_error.h \
include/ft_manual_reset_event.h \
include/ft_bmp.h \
include/ft_list.h \
include/ft_shared_ptr.h \
include/ft_aabb.h \
include/ft_get_next_line_int.h \
include/ft_quaternion.h \
include/ft_vec3.h \
include/ft_time.h \
include/libft.h \
include/ft_bmp_int.h \
include/ft_parse_utils.h \
include/ft_linked_list.h \
include/ft_printf.h \
include/ft_vec2.h \
include/ft_matrix.h \
include/ft_init_create_threading.h \
include/ft_get_next_line.h

ifdef DEBUG
FLAGS += -g
USE_OBJ_DIR = $(DEBUG_DIR)
OTHER_OBJ_DIR = $(OBJ_DIR)
else
FLAGS += -O2
USE_OBJ_DIR = $(OBJ_DIR)
OTHER_OBJ_DIR = $(DEBUG_DIR)
endif

OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(USE_OBJ_DIR)%.o)

.PHONY: all
all:
	$(MAKE) $(NAME)

.PHONY: debug
debug:
	$(MAKE) DEBUG=1

# because we have used the S flag, it does not have a index, (faster)
# run ar with the s flag to create the index
$(NAME): $(OBJECTS) Makefile | $(OBJ_DIR) $(DEBUG_DIR)
	ar -rc $(NAME) $(OBJECTS)
	# mark non-debug or debug dirty, after making libft.a
	touch -c $$(find $(OTHER_OBJ_DIR))

$(OBJ_DIR) $(DEBUG_DIR) $(SRC_DIR):
	mkdir $@

$(OBJECTS): $(USE_OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES) Makefile \
	| $(SRC_DIR) $(USE_OBJ_DIR)
	mkdir -p $(shell dirname $@)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(DEBUG_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re:
	$(MAKE) fclean
	$(MAKE) all
