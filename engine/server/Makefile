##
## EPITECH PROJECT, 2023
## B-YEP-400-LYN-4-1-engine-nelly.pereira-alves
## File description:
## Makefile
##

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I ./include
LDFLAGS = -luuid -lm

SRC_DIR = ./src/server

SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)

BUILDDIR = ./obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

BUFFER_LIB = ./src/circular_buffer
NETWORK_LIB = ./src/network

LIB_DIR = ./libs
LIBS = -L$(LIB_DIR) -lnetwork -lcircular_buffer

TARGET = engine_server

ECHO = echo -e
RED = \033[0;31m\033[1m
GREEN = \033[0;32m\033[1m
WHITE = \033[1;37m\033[1m
NC = \033[0m

$(BUILDDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -MD -c $< $(CFLAGS)

all: libs_target $(TARGET)

libs_target:
	@$(ECHO) "$(WHITE)[BUILD LIBS]$(NC)"
	$(MAKE) --no-print-directory -C $(BUFFER_LIB) && \
		$(ECHO) "$(GREEN)[OK]$(NC)" || $(ECHO) "$(RED)[FAILED]$(NC)"
	$(MAKE) --no-print-directory -C $(NETWORK_LIB) && \
		$(ECHO) "$(GREEN)[OK]$(NC)" || $(ECHO) "$(RED)[FAILED]$(NC)"

$(TARGET): $(OBJ)
	@$(ECHO) "$(WHITE)[BUILD]$(NC)"
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS) -Wl,-rpath,$(LIB_DIR)

clean:
	@$(ECHO) "$(WHITE)[CLEAN]$(NC)"
	rm -rf $(BUILDDIR)
	$(MAKE) --no-print-directory -C $(BUFFER_LIB) $@
	$(MAKE) --no-print-directory -C $(NETWORK_LIB) $@

fclean: clean
	@$(ECHO) "$(WHITE)[FCLEAN]$(NC)"
	rm -rf $(TARGET) $(LIB_DIR)/libcircular_buffer.so $(LIB_DIR)/libnetwork.so
	$(MAKE) --no-print-directory -C $(BUFFER_LIB) $@
	$(MAKE) --no-print-directory -C $(NETWORK_LIB) $@

re: fclean all

.PHONY: clean fclean re
