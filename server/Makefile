compiler = g++

Server =  exec_server

clean:
	rm -f $(Server) 

all :  $(Server)

$(Server):
		$(compiler) -w ./server/Server.cpp -o $(Server) -pthread; ./exec_server
