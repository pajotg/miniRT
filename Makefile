NAME = miniRT

# since i wrote a nice tester that i am happy with
# i want to use it for this project too!
# but it needs a library or c files to compile the test with
# so here is it!
TEST_BINDER_NAME = TestBinder.a

OS = $(shell uname -s)
ifeq ($(OS), Darwin)
MINI_LIBX_DIR = minilibx_mac
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
LDFLAGS = -L$(MINI_LIBX_DIR)/ -lmlx -Llibft -lft -lm -lpthread

FOREIGN_TARGETS = $(MINI_LIBX_DIR)/libmlx.a libft/libft.a
MAKE_FOREIGN_TARGETS = $(MINI_LIBX_DIR)/libmlx libft/libft

FLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror
#FLAGS = -DBUFFER_SIZE=128 # warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning warning

FLAGS += -D OS_$(OS)

ifeq ($(OS), Darwin)
LDFLAGS += -framework OpenGL -framework AppKit
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

SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)
HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name *.h)
OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(USE_OBJ_DIR)%.o)

.PHONY: all
all: $(NAME)

.PHONY: debug
debug:
	$(MAKE) DEBUG=1

# Be sure to make libft and minilibx no longer submodules when you want to eval, it dont work when you clone it! REEE
# Also be sure to find #include <stdio.h>, you never know where i left it in, alright? alright.
# Making it work with the foreign targets was a nightmare, the phony on the $(MAKE) -C libft make it look always out of date
# so it would always re-link, so i had to put the MAKE_FOREIGN_TARGETS in the order only dependency so it would not matter if it was "out of date"
# and then ALSO add the real library in the dependencies so it would update if it actually was out of date... UGH!
$(NAME): $(OBJECTS) $(FOREIGN_TARGETS) | $(MAKE_FOREIGN_TARGETS) $(OBJ_DIR) $(DEBUG_DIR)
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


# only the other makefile can determine if it needs to be updated, so phony it.
# kinda annoying that i had to seperate the real target and the "call make target"
#	i chose to do it by removing the .a at the end
# maybe not the best way, oh well

.PHONY: $(MAKE_FOREIGN_TARGETS)
$(MAKE_FOREIGN_TARGETS):
	#$(MAKE) -C $(dir $@) $(notdir $@).a
	$(MAKE) -C $(dir $@)

# also in case the file does not exist yet, add this rule so it wont error
# we will make the libs with the $(MAKE_FOREIGN_TARGETS) target
$(FOREIGN_TARGETS):


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