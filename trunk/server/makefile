CC = g++ -g  -lrt

CXXFLAGS =  -O2 -Wall -Wno-deprecated  

NET = ../net

SYS = ../sys

objects =service.o transport.o tcpcomponent.o tcpaccepter.o tcpconnection.o iocomponent.o epollsocketevent.o	serversocket.o	socket.o socketevent.o connection.o  packetqueuethread.o packetqueue.o packet.o  timeutil.o defaultrunnable.o netutil.o log.o warningBuffer.o

all : tcp_server 

service.o : service.cpp   
	$(CC) $(CXXFLAGS) \
	-c service.cpp  

tcp_server : $(objects)  
	$(CC)  $(objects)    -lpthread \
	-o tcp_server 

transport.o : $(NET)/transport.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/transport.cpp  

tcpcomponent.o : $(NET)/tcpcomponent.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/tcpcomponent.cpp  

tcpaccepter.o : $(NET)/tcpaccepter.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/tcpaccepter.cpp  

tcpconnection.o : $(NET)/tcpconnection.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/tcpconnection.cpp  

connection.o : $(NET)/connection.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/connection.cpp  

iocomponent.o : $(NET)/iocomponent.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/iocomponent.cpp  

serversocket.o :$(NET)/serversocket.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/serversocket.cpp 	

socket.o :$(NET)/socket.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(NET)/socket.cpp  

packetqueuethread.o :$(NET)/packetqueuethread.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(NET)/packetqueuethread.cpp  

packetqueue.o :$(NET)/packetqueue.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(NET)/packetqueue.cpp  

packet.o :$(NET)/packet.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(NET)/packet.cpp  

socketevent.o :$(NET)/socketevent.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/socketevent.cpp 	

epollsocketevent.o : $(NET)/epollsocketevent.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(NET)/epollsocketevent.cpp  

timeutil.o :$(SYS)/timeutil.cpp   
	$(CC)  $(CXXFLAGS) \
	-c -lrt $(SYS)/timeutil.cpp  

defaultrunnable.o :$(SYS)/defaultrunnable.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(SYS)/defaultrunnable.cpp  

netutil.o :$(SYS)/netutil.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(SYS)/netutil.cpp 

log.o :$(SYS)/log.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  $(SYS)/log.cpp  

warningBuffer.o :$(SYS)/warningBuffer.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  $(SYS)/warningBuffer.cpp  

clean:
	rm -f $(objects) 

