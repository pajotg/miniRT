NAME = miniRT

# since i wrote a nice tester that i am happy with
# i want to use it for this project too!
# but it needs a library or c files to compile the test with
# so here is it!
TEST_BINDER_NAME = TestBinder.a

INC=%%%%

INCLIB=$(INC)/../lib

SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIRS = include/
INCLUDE = -Iinclude -Ilibft/include -Iminilibx-linux -Isrc/get_next_line
LDFLAGS = -Lminilibx-linux/ -lmlx -Llibft -lft -L$(INCLIB) -lXext -lX11 -lm -lbsd -lpthread

FLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror -O2

# Really not really happy where the GNL is now located, TODO: Add makefile into GNL that compiles a library, and then link ik via -L and -l

SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)	# Really long comment so i will fix it once i am done with making it work alright okay this should trigger the norminette+
HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name *.h)
OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

.PHONY: all
all: $(NAME)

# Be sure to make libft and minilibx no longer submodules when you want to eval, it dont work when you clone it! REEE
# Also be sure to find #include <stdio.h>, you never know where i left it in, alright? alright.
$(NAME): $(OBJECTS)
	$(MAKE) -C minilibx-linux
	$(MAKE) -C libft
	$(CC) $(FLAGS) -o $(NAME) $^ $(LDFLAGS)

$(TEST_BINDER_NAME): $(OBJECTS)
	$(MAKE) -C minilibx-linux
	$(MAKE) -C libft
	rm -f $(TEST_BINDER_NAME)
	ar -rcS $(TEST_BINDER_NAME) $(filter-out %main.o,$(OBJECTS))
	ranlib $(TEST_BINDER_NAME) # Really long comment so i will fix it once i am done with making it work alright okay this should trigger the norminette+

$(OBJ_DIR) $(SRC_DIR):
	mkdir $@

$(OBJECTS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES) \
	| $(SRC_DIR) $(OBJ_DIR)
	mkdir -p $(shell dirname $@)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_BINDER_NAME)

.PHONY: re
re:
	$(MAKE) fclean
	$(MAKE) all