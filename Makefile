NAME := woody_woodpacker

CC := cc

CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -Iinclude -MMD -MP

LDFLAGS := 
LDLIBS := $(addprefix -l, c)

SRC_DIR := src
INC_DIR := include
BUILD_DIR := .build

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DOCKER_IMAGE := $(NAME)
DOCKER_FLAGS := --cap-add=NET_RAW --rm -it -v $(shell pwd):/app -w /app

.PHONY: all clean fclean re docker-build docker-dev docker-clean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

docker-build:
	docker build --target dev -t $(DOCKER_IMAGE):dev .

docker-dev: docker-build
	docker run $(DOCKER_FLAGS) $(DOCKER_IMAGE):dev bash

docker-clean:
	docker rmi -f $(DOCKER_IMAGE):dev
