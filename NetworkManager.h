#ifndef _NetworkManager_H_
#define _NetworkManager_H_

#include <SDL/SDL_net.h>
#include <gameUpdate.h>



class NetworkManager {
    
    public:   
		NetworkManager();
		virtual ~NetworkManager();
		void checkForClient();
		bool checkForServer();
		bool checkForPackets();
		bool sendPacket(gameUpdate update);
		void broadcastToClients(const char* host);
		gameUpdate* getGameUpdate();
		bool isConnectionOpen();
		bool isThisServer();
		void waitForClientConnection();
		char* intToIpAddr(long ipAddress);
		char* intToIpAddr(long ipAddr, bool networkByteOrder);
		int getMyIp();
		int getTheirIp();
		
	private:
		void readPacketToBuffer();
		bool connectionOpen;
		bool isServer;
		
};

#endif 
