# Put your user name below:
USER= seavera
CC= g++ -std=c++11

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: pongServer pongAjax PutCGI PutHTML
#all: pongServer 

pongServer.o: pongServer.cpp fifo.h
	$(CC) -c $(CFLAGS) pongServer.cpp

pongAjax.o: pongAjax.cpp fifo.h
	$(CC) -c $(CFLAGS) pongAjax.cpp

pongServer: pongServer.o fifo.o
	$(CC) pongServer.o  fifo.o -o pongServer

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp

pongAjax: pongAjax.o  fifo.o
	$(CC) pongAjax.o  fifo.o -o pongAjax -L/usr/local/lib -lcgicc

PutCGI: pongAjax
	chmod 757 pongAjax
	cp pongAjax /usr/lib/cgi-bin/$(USER)_pongAjax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp pongWebpage.css /var/www/html/class/softdev/$(USER)/project4
	cp pongWebpage.html /var/www/html/class/softdev/$(USER)/project4
	cp pong.js  /var/www/html/class/softdev/$(USER)/project4

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)/project4

clean:
	rm -f *.o pongServer pongAjax

