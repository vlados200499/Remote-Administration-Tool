
#include "Server.h"

#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <commdlg.h>
std::vector<std::thread>clientThreads;




void Server::HandleClients()
{
	while (true)
	{
		
		for (size_t i = 0; i < Client_Server_Information.size(); i++)
		{
			auto device = Client_Server_Information[i].first;
			auto socket = Client_Server_Information[i].second;
			
			
			


		}
		
	}
	


}

inline int Server::GenerateID(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
	return 0;
}

void Server::StartServer(std::string IP, int port)
{

	
		
	


	this->IP = IP;
	this->port = port;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		
		exit(1);
	}


	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	getaddrinfo(this->IP.c_str(), std::to_string(this->port).c_str(), &hints, &result);

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	
	listen(ListenSocket, 100);





}


void Server::start_server_for_listen_new_clients()
{
	
	
	for (;;)
	{
		
		
		if (ClientSocket != INVALID_SOCKET) 
		{
				ClientSocket = accept(ListenSocket, NULL, NULL);
				
				client.id = std::to_string(GenerateID(100, 10000));
				
				Client_Server_Information.emplace_back(client, ClientSocket);
				
				clientThreads.emplace_back(&Server::HandleClients,this);
				
				       
				
				
		}
		
		
	
	
	}
				

				

				
				
					
}
void Server::SendFile(SOCKET cl)
{
	
	char filePath[MAX_PATH];
	char buffer[4096];
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = sizeof(buffer);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        // Open the selected file
        HANDLE hFile = CreateFileA(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        
    	// Get the size of the file
    	DWORD fileSize = GetFileSize(hFile, NULL);
		fileSize++;
            // Read the contents of the file
    	DWORD bytesRead;
    	ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);

		
            // Close the file
    	CloseHandle(hFile);
		
    	send(cl,std::to_string(fileSize).c_str(),sizeof(fileSize),0);
    	send(cl,buffer,fileSize,0);
        
    }

}

void Server::SendFunction(SOCKET cl,PacketType paket ,std::string msg)
{
	switch (paket)
	{
	case cmd:
		send(ClientSocket, "CMD",sizeof("CMD") , 0);
		send(ClientSocket, msg.c_str(), msg.size()+1, 0);
		break;
	case file:
		send(ClientSocket, "File",sizeof("File") , 0);	
		SendFile(cl);
		break;
	}
    
	
}










Server::Server()
{
	for (auto& thread : clientThreads) {
		thread.join();
	}
}

Server::~Server()
{


}



