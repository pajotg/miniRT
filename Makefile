NAME = miniRT

# since i wrote a nice tester that i am happy with
# i want to use it for this project too!
# but it needs a library or c files to compile the test with
# so here is it!
TEST_BINDER_NAME = TestBinder.a

SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = include/

FLAGS = -Wall -Wextra -Werror

SOURCE_FILES = $(shell find src -type f -name *.c)	# Really long comment so i will fix it once i am done with making it work alright okay this should trigger the norminette+
HEADER_FILES = $(shell find $(INCLUDE_DIR) -type f -name *.c)
OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS)

$(OBJ_DIR):
	mkdir $@

$(OBJ_DIR)%.o: %.c $(HEADER_FILES) \
	| $(SRC_DIR) $(OBJ_DIR)
	$(CC) $(FLAGS) -I $(INCLUDE_DIR) -c -o $@ $<
	ar -rc $(NAME) $@