
all: tcp upd

tcp: tcp_client tcp_server

upd: upd_client upd_server

tcp_client: tcpclient.c
	gcc -o tcpclres tcpclient.c

upd_client: udpclient.c
	gcc -o udpclres udpclient.c

tcp_server: tcpserver.c
	gcc -o tcpsres tcpserver.c

upd_server: udpserver.c
	gcc -o udpsres udpserver.c

clean:
	rm *.out *.o -f
