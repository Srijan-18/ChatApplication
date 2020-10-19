compiler = g++

Client = exec_client

clean:
	rm -f $(Client)

all :  $(Client)

$(Client):
		$(compiler) -w ./controller/ClientController.cpp ./model/ClientService.cpp ./model/ClientService.h ./view/clientView.cpp ./view/clientView.h -o $(Client) -pthread
