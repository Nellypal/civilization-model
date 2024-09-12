##
## EPITECH PROJECT, 2023
## B-YEP-400-LYN-4-1-zappy-nelly.pereira-alves
## File description:
## Makefile
##

AI_DIR = ./model
ENGINE_DIR = ./engine

TARGET_AI = civilization
TARGET_SERVER = engine_server

TARGET_EGINE = engine_build

ECHO = echo -e
RED = \033[0;31m\033[1m
GREEN = \033[0;32m\033[1m
WHITE = \033[1;37m\033[1m
NC = \033[0m

all: engine_build $(TARGET_AI)

$(TARGET_AI):
	@$(ECHO) "$(WHITE)[BUILD AI]$(NC)"
	ln -sf $(AI_DIR)/$(TARGET_AI) ./$(TARGET_AI) && \
		$(ECHO) "$(GREEN)[OK]$(NC)" || $(ECHO) "$(RED)[FAILED]$(NC)"

engine_build:
	@$(ECHO) "$(WHITE)[BUILD ENGINE]$(NC)"
	$(MAKE) --no-print-directory -C $(ENGINE_DIR) && \
		mv $(ENGINE_DIR)/$(TARGET_SERVER) . && \
		cp -r $(ENGINE_DIR)/libs . && \
			$(ECHO) "$(GREEN)[OK]$(NC)" || $(ECHO) "$(RED)[FAILED]$(NC)"

clean:
	@$(ECHO) "$(WHITE)[CLEAN]$(NC)"
	$(MAKE) --no-print-directory -C $(ENGINE_DIR) $@

fclean: clean
	@$(ECHO) "$(WHITE)[FCLEAN]$(NC)"
	$(MAKE) --no-print-directory -C $(ENGINE_DIR) $@
	rm -f $(TARGET_AI)
	rm -f $(TARGET_SERVER)
	rm -rf ./libs

re: fclean all

.PHONY: clean fclean re
