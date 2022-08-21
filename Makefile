all:
	@make -C server
	@make -C client
	

client:
	@make -C client $@

server:
	@make -C server $@

clean:
	@make -C server clean
	@make -C client clean

uninstall: 
	@make -C server uninstall
	@make -C client uninstall

reinstall: uninstall all
