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

SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)
HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name *.h)

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