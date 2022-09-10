
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <chrono>

using namespace std;


int main()
{
	/******************************************/
			//Winsock Initialization 
	/******************************************/
	WSADATA wsad;  // Vairable of type WSADATA that contain the WinSock Data
	SOCKET ClientSocket; // Variables of type Socket , ClientSocket
	int wsaerror, port = 55000;		    // Port Number To communicate through,wsaerr is an Integr variable to carry the Return of WSASTARTUP
	WORD wsaVer;				        // Winsock Version Variable
	sockaddr_in ClientStruct;			// Socket Structure that Hold the Socket Data ex:Family Address , IP and Port

	
	wsaVer = MAKEWORD(2, 2);           // Assign Version Type to wsaVer

	/******************************************/
   //           Initalize Win Socket
   /******************************************/
	wsaerror = WSAStartup(wsaVer, &wsad);// WSASTARTUP function take 2 parameters , First:winsock Version and Second:Address of the WSADATA Variable

	if (wsaerror != 0)					  // WSASTARTUP Return zero incase of Successful so this is check on the return Value
	{
		cout << "Client Failed to Initialize Win Socket" << endl;
	}
	else
	{
		cout << "Client Win Socket is Ok" << endl;
	}

	/******************************************/
	//           Initalize CLient Socket
	/******************************************/
	ClientSocket = INVALID_SOCKET;		//Assign the Value of ServerSocket to INVALID_SOCKET so we can check on its value after calling socket function()
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Call socket() Function that Create the ServerSocket, Function Arguments are Family Address which is AF_INET for TCP
															  //  SOCK_STREAM and IPPROTO_TCP for TCP Protocol

	if (ClientSocket == INVALID_SOCKET)						// Check on the Return value of socket() function to make sure that the socket is created successfuly
	{
		cout << "Client Socket Failed to Initialize" << endl;
	}
	else
	{
		cout << "Client Socket Created Successfuly" << endl;
	}

	/*****************************************/
    //    Connect the Client to the Server
    /****************************************/
	ClientStruct.sin_port = htons(port);        //Write Port Value in SocketStruct that Hold ServerSocket Data , htons to change the Byte Order (Big and Little Endian)
	ClientStruct.sin_family = AF_INET;			//Write the ServerSocket Family Address in ServerStruct
	inet_pton(AF_INET, "127.0.0.1", &ClientStruct.sin_addr);	//Write the IP Address in ServerStruct

	if (connect(ClientSocket, (struct sockaddr*)&ClientStruct, sizeof(ClientStruct)) != 0)
	{
		cout << "Client Failed to Connect" << endl;
	}
	else
	{
		cout << "Client Connected to Server" << endl;
	}

	/*****************************************/
    //    Send Data From Client to Server 
    /****************************************/
	short value[20]={1,2,3,4,5,6,55,25,65,10,22,33,77,88,99,44,66,33,88,20};  //Array of Temperature Values to be sent to Client
	auto start = std::chrono::high_resolution_clock::now();		//Variable that will contain the Initiale time 
	char i = 0;
	while (1)
	{
		/*
		* sebd() that responsible for Sending data from client to Server
		* First parameter : the ClientSocket
		* Second : the Address of the Hold Variable casted to char in case of different type
		* third is the Size of Hold Variable
		* Forth : Flag
		*/
		auto current = std::chrono::high_resolution_clock::now();   //Get the Current time	
		std::chrono::duration<float> elapsed = current - start;		//Calculate the Difference between Start time and Current time to Print the Values after 5 seconds Difference
		
		if (elapsed.count() >= 1.0)
		{
			std::cout << "One Second Elapsed, New Value Sent to Server" << std::endl;
			start = current;		//Assign the Start Time to be the Current time for next comparison

			send(ClientSocket, (char*)&value[i++], sizeof(short), 0);
		}
		if (i == 20)   // This condition created to to repeat sending the array again
		{
			i = 0;
		}
	}

	WSACleanup();

}
