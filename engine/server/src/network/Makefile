CC = gcc
CFLAGS = -fPIC -Wall -Wextra -Werror -g3 -I ../../include
LDFLAGS = -shared

SRC_DIR = .

SRC := $(wildcard $(SRC_DIR)/*.c)

BUILDDIR = ./obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

TARGET = ../../libs/libnetwork.so

$(BUILDDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -MD -c $< $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: clean fclean re
