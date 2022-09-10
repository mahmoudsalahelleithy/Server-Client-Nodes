
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
	SOCKET ServerSocket, ReceiveSockect; // Variables of type Socket , ServerSocket is the orginal one and ReceiveSocket will be Created after Listening from Client
	int wsaerror, port = 55000;		    // Port Number To communicate through,wsaerr is an Integr variable to carry the Return of WSASTARTUP
	WORD wsaVer;				        // Winsock Version Variable
	sockaddr_in ServerStruct;			// Socket Structure that Hold the Socket Data ex:Family Address , IP and Port

	wsaVer = MAKEWORD(2, 2);           // Assign Version Type to wsaVer

	/******************************************/
	//           Initalize Win Socket
	/******************************************/
	wsaerror = WSAStartup(wsaVer, &wsad);  // WSASTARTUP function take 2 parameters , First:winsock Version and Second:Address of the WSADATA Variable

	if (wsaerror != 0)					  // WSASTARTUP Return zero incase of Successful so this is check on the return Value
	{
		cout << "Server Failed to Initialize Win Socket" << endl;
	}
	else
	{
		cout << "Server Win Socket is Ok" << endl;
	}
	/******************************************/
	//         Create Server Socket
	/******************************************/
	ServerSocket = INVALID_SOCKET;		//Assign the Value of ServerSocket to INVALID_SOCKET so we can check on its value after calling socket function()
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Call socket() Function that Create the ServerSocket, Function Arguments are Family Address which is AF_INET for TCP
	//  SOCK_STREAM and IPPROTO_TCP for TCP Protocol
	if (ServerSocket == INVALID_SOCKET)						// Check on the Return value of socket() function to make sure that the socket is created successfuly
	{
		cout << "Server Socket Failed to Initialize" << endl;
	}
	else
	{
		cout << "Server Socket Created Successfuly" << endl;
	}
	/*****************************************/
	//           Bind the Socket
	/****************************************/
	ServerStruct.sin_port = htons(port);		//Write Port Value in SocketStruct that Hold ServerSocket Data , htons to change the Byte Order (Big and Little Endian)
	ServerStruct.sin_family = AF_INET;			//Write the ServerSocket Family Address in ServerStruct 
	ServerStruct.sin_addr.s_addr = INADDR_ANY;	//Write the IP Address in ServerStruct

	if (bind(ServerSocket, (struct sockaddr*)&ServerStruct, sizeof(ServerStruct)) != 0)
	{															//Bind Function to assign the socket to its IP and Port
		cout << "Server Binding Failed" << endl;				//First Parameter is the Server Sokcet Created before
	}															//Second is the ServerSocket Structure that Hold its Data which is ServerSTruct	
	else                                                        //Third is Sizeof the Structure
	{
		cout << "Server Binding is Ok" << endl;
	}

	/*****************************************/
	//          Initialize Listen
	/*****************************************/
	if (listen(ServerSocket, 1) == SOCKET_ERROR)      //Listen() Function Called to Initialize the Connection Between Server and Client
	{												  //First Parameter is the Socket Created before ,Second is the Number of Clients can Connect to it
		cout << "Server Listen Function Failed" << endl;
	}
	else
	{
		cout << "Server Listen Initialized" << endl;
	}

	/*****************************************/
	//     Accept Sokcet Function
	/*****************************************/
	ReceiveSockect = accept(ServerSocket, NULL, NULL);	//accept Function will Hold the Excution of code until connected to a client
	//it take the Socket created as first argument,Second and third are client data which is not required
	if (ReceiveSockect == INVALID_SOCKET)				//Check the return type of accept function
	{
		cout << "Server Accept Function Failed" << endl;
	}
	else
	{
		cout << "Server Accept Function Initialized" << endl;
	}
	/*****************************************/
	//         Receive from Client
	/*****************************************/
	short number;		//Variable that will hold the Value sent from Client
	short sum = 0;		//Variable to calculate the accumaltive Value Received from client 
	short index = 5;	//Index with intial value of 5 which will be increaed by 5 every 5 Seconds
	auto start = std::chrono::high_resolution_clock::now();		//Variable that will contain the Initiale time 

	while (1)
	{
		/*
		* recv() that responsible for Receiving data from client
		* First parameter : the ReceiveSocket Created from accept Function
		* Second : the Address of the Hold Variable casted to char in case of different type
		* third is the Size of Hold Variable
		* Forth : Flag
		*/
		recv(ReceiveSockect, (char*)&number, sizeof(short), 0);
		sum += number;											    //calculate the total value of Received Temperature from Client						
		auto current = std::chrono::high_resolution_clock::now();   //Get the Current time	
		std::chrono::duration<float> elapsed = current - start;		//Calculate the Difference between Start time and Current time to Print the Values after 5 seconds Difference
		if (elapsed.count() >= 5.0)
		{
			printf("Accumulative Value From Client : %d Celsius\nAverage Value in %d Seconds is : %d Celsius\n", sum, index, (sum / index));
			start = current;		//Assign the Start Time to be the Current time for next comparison
			index += 5;				//Increase the Index by 5 which is the Last 5 seconds
		}
	}

	WSACleanup();
}