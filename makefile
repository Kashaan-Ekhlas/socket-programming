server: 
	gcc server_side.c -L. -labstractsocks -o server_side

client: 
	gcc client_side.c -L. -labstractsocks -o client_side

clean_server: 
	rm -f server_side
	
clean_client: 
	rm -f client_side