CC = g++ -g  -lrt

CXXFLAGS =  -O2 -Wall -Wno-deprecated  

MAANET = ../net

MAASYS = ../sys

objects = server.o service.o packetstreamer.o transport.o epollsocketevent.o tcpcomponent.o tcpaccepter.o tcpconnection.o iocomponent.o serversocket.o socketevent.o connection.o socket.o packetqueuethread.o packetqueue.o packet.o defaultrunnable.o netutil.o parameter.o timeutil.o config.o func.o log.o warningBuffer.o 

all : tcp_server 

tcp_server : $(objects)  
	$(CC)  $(objects)    -lpthread \
	-o tcp_server 

server.o : server.cpp   
	$(CC) $(CXXFLAGS) \
	-c server.cpp  

service.o : service.cpp   
	$(CC) $(CXXFLAGS) \
	-c service.cpp  

packetstreamer.o : packetstreamer.cpp 
	$(CC) $(CXXFLAGS)  \
	-c packetstreamer.cpp  

transport.o : $(MAANET)/transport.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/transport.cpp  

epollsocketevent.o : $(MAANET)/epollsocketevent.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/epollsocketevent.cpp  

tcpcomponent.o : $(MAANET)/tcpcomponent.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/tcpcomponent.cpp  

tcpaccepter.o : $(MAANET)/tcpaccepter.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/tcpaccepter.cpp  

tcpconnection.o : $(MAANET)/tcpconnection.cpp 
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/tcpconnection.cpp  

iocomponent.o : $(MAANET)/iocomponent.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/iocomponent.cpp  

connection.o : $(MAANET)/connection.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/connection.cpp  

socketevent.o :$(MAANET)/socketevent.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/socketevent.cpp 	

serversocket.o :$(MAANET)/serversocket.cpp  
	$(CC) $(CXXFLAGS)  \
	-c $(MAANET)/serversocket.cpp 	

socket.o :$(MAANET)/socket.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAANET)/socket.cpp  

packetqueuethread.o :$(MAANET)/packetqueuethread.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAANET)/packetqueuethread.cpp  

packetqueue.o :$(MAANET)/packetqueue.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAANET)/packetqueue.cpp  

packet.o :$(MAANET)/packet.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAANET)/packet.cpp  

defaultrunnable.o :$(MAASYS)/defaultrunnable.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAASYS)/defaultrunnable.cpp  
	
netutil.o :$(MAASYS)/netutil.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAASYS)/netutil.cpp 
 
parameter.o :$(MAASYS)/parameter.cpp   
	$(CC) $(CXXFLAGS) \
	-c $(MAASYS)/parameter.cpp 

timeutil.o :$(MAASYS)/timeutil.cpp   
	$(CC)  $(CXXFLAGS) \
	-c -lrt $(MAASYS)/timeutil.cpp  

config.o :config.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  config.cpp  

func.o :func.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  func.cpp  

log.o :$(MAASYS)/log.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  $(MAASYS)/log.cpp  

warningBuffer.o :$(MAASYS)/warningBuffer.cpp   
	$(CC)  $(CXXFLAGS) \
	-c  $(MAASYS)/warningBuffer.cpp  
clean:
	rm -f $(objects) 
	rm -f tcp_server 
