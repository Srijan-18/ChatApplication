compiler = g++

Server =  exec_server

clean:
	rm -f $(Server) 

all :  $(Server)

$(Server):
		$(compiler) -w ./controller/main.cpp ./controller/server_controller.cpp ./controller/server_controller.h ./model/ServerService.cpp ./model/ServerService.h ./view/server_view.h ./view/server_view.cpp ../client/model/ClientStructure.cpp -o $(Server) -pthread; ./exec_server
		