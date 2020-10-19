compiler = g++

Client = exec_client

clean:
	rm -f $(Client)

all :  $(Client)

$(Client):
		$(compiler) -w ./client/controller/ClientController.cpp ./client/model/ClientService.cpp ./client/model/ClientService.h ./client/view/clientView.cpp ./client/view/clientView.h -o $(Client) -pthread
