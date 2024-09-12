SERVER_DIR = ./server

TARGET_SERVER = engine_server

ECHO = echo -e
RED = \033[0;31m\033[1m
GREEN = \033[0;32m\033[1m
WHITE = \033[1;37m\033[1m
NC = \033[0m

all: $(TARGET_SERVER)

$(TARGET_SERVER):
	@$(ECHO) "$(WHITE)[BUILD SERVER]$(NC)"
	$(MAKE) --no-print-directory -C $(SERVER_DIR) && \
		mv $(SERVER_DIR)/$(TARGET_SERVER) . && \
		cp -r $(SERVER_DIR)/libs . && \
			$(ECHO) "$(GREEN)[OK]$(NC)" || $(ECHO) "$(RED)[FAILED]$(NC)"

clean:
	@$(ECHO) "$(WHITE)[CLEAN]$(NC)"
	$(MAKE) --no-print-directory -C $(SERVER_DIR) $@

fclean: clean
	@$(ECHO) "$(WHITE)[FCLEAN]$(NC)"
	$(MAKE) --no-print-directory -C $(SERVER_DIR) $@
	rm -f $(TARGET_SERVER)
	rm -rf ./libs

re: fclean all

.PHONY: clean fclean re
