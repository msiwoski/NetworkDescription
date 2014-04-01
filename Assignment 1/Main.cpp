/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Main.cpp
--
-- PROGRAM: Main
--
-- FUNCTIONS:
-- int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lspszCmdParam, int nCmdShow)
-- BOOL Register(HINSTANCE hInst)
-- HWND Create(HINSTANCE hInst, int nCmdShow)
-- LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
-- BOOL Window_OnCreate(HWND hwnd)
-- void Window_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
-- void Window_OnDestroy (HWND hwnd)
-- void Window_OnPaint(HWND hwnd)
-- BOOL CALLBACK IPDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
-- BOOL CALLBACK HostDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
-- BOOL CALLBACK PortNumDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
-- BOOL CALLBACK ServiceNameDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--
-- DATE: January 12, 2014
--
-- REVISIONS:  
-- January 12, 2014 - Mat Siwoski: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- NOTES:
-- This Program will allow you to retrieve information using Sockets. The information that will be retrieved
-- will be dependent on the information entered. 
-- The four following options will be available:
-		- Get the Host Name by an IP Address
--		- Get the IP Address by the Host Name
--		- Get a Port Number by the Service Name and Protocol Type.
--		- Get a Service Name by the Port Number and Protocol Type.
-- All information will be printed for the option selected.
--
-- Remember to add "WS2_32.Lib" to the project source list.
--
----------------------------------------------------------------------------------------------------------------------*/

#include "Resources.h"
#include <stdio.h>

static HWND MainWindow;
static char szAppName[] = "Windows Protocol";
static HINSTANCE hInstance;
LPSTR pszFileText;
static TCHAR szChildClass[]  = TEXT ("ChildClass");
HINSTANCE	hInst ;
HMENU hMenuInit;
HWND hwndClient;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: January 12, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lspszCmdParam, int nCmdShow)
--				HINSTANCE hInst: Handle to the current instance of the program.
--				HINSTANCE hPrevInstance: Handle to the previous instance of the program.
--				LPSTR lspszCmdParam: Command line for the application.
--				int nCmdShow: Control for how the window should be shown.
--
-- RETURNS: Returns the exit value upon exit.
--
-- NOTES:
-- This function is the entry point for a graphical Windows-based application.
------------------------------------------------------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lspszCmdParam, int nCmdShow){
	MSG Msg;

	if (!hPrevInstance){
            if (!Register(hInst)){
                    return FALSE;
            }
    }
    MainWindow = Create (hInst, nCmdShow);
    if (!MainWindow){
            return FALSE;
    }

	while (GetMessage (&Msg, NULL, 0, 0))
    {
            TranslateMessage (&Msg);
            DispatchMessage (&Msg);
    }
    return Msg.wParam;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Register
--
-- DATE: January 12, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL Register(HINSTANCE hInst)
--				HINSTANCE hInst: Handle to the window
--
-- RETURNS: Returns true if the registration has passed, otherwise, false
--
-- NOTES:
-- This function registers the parameters for the intial window of the main program. 
------------------------------------------------------------------------------------------------------------------*/
BOOL Register(HINSTANCE hInst)
{
	WNDCLASS W;
	memset (&W, 0, sizeof(WNDCLASS));
	W.style			= CS_HREDRAW | CS_VREDRAW;
	W.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	W.hCursor		= LoadCursor (NULL, IDC_ARROW);
	W.lpfnWndProc	= WndProc;
	W.hInstance		= hInst;
	W.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	W.lpszClassName = szAppName;
	W.cbClsExtra	= 0;
	W.cbWndExtra	= sizeof(VariablesNeeded*);
	W.lpszMenuName	= TEXT("MYMENU");

	return (RegisterClass (&W) != 0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Create
--
-- DATE: January 12, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: HWND Create(HINSTANCE hInst, int nCmdShow)
--				HINSTANCE hInst: A handle to the window.
--				int nCmdShow: Control how the window should be shown.
--
-- RETURNS: Returns the handle to the window.
--
-- NOTES:
-- This function creates the main window of the application. As well, memory will be allocated for the structs
-- used with the socket programming.
------------------------------------------------------------------------------------------------------------------*/
HWND Create(HINSTANCE hInst, int nCmdShow)
{
	VariablesNeeded* varNeeded;
	HWND hwnd = CreateWindow (szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, hMenuInit, hInst, NULL);
	if (hwnd == NULL)
		return hwnd;
	nCmdShow = SW_SHOW;
	varNeeded = (VariablesNeeded*) malloc(sizeof(VariablesNeeded));
	varNeeded->hp = (struct hostent*) malloc(MAXGETHOSTSTRUCT);
	varNeeded->sv = (struct servent*) malloc(MAXGETHOSTSTRUCT);
	SetWindowLongPtr(hwnd, 0, (long)varNeeded);
	for(int i = 0; i < 50; i++){
		varNeeded->tmp[i] = ' ';
	}
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	return hwnd;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--				HWND hwnd: Handle to the window
--				UINT Message: The message
--				WPARAM wParam: Parameter
--				LPARAM lParam: Parameter
--
-- RETURNS: 
--
-- NOTES:
-- This function that handles the different type of window messages.
------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	VariablesNeeded* varNeeded = (VariablesNeeded*) GetWindowLongPtr(hwnd, 0); 
	HDC hdc;
	//char tmp[50];				//Temp for the aliases
	char **p;
	int i = 0;
	RECT rect;
	PAINTSTRUCT paint;
	switch (Message){
		HANDLE_MSG(hwnd, WM_CREATE, Window_OnCreate);
		HANDLE_MSG(hwnd, WM_COMMAND, Window_OnCommand);
		HANDLE_MSG(hwnd, WM_DESTROY, Window_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Window_OnPaint);
	case IP_HOST_MSG:
		//error checking
		if (HIWORD(lParam) != 0)
		{
			switch(HIWORD(lParam))
			{
				case WSAHOST_NOT_FOUND:
					printf("ERROR: Host was not found. Try Again");
					break;
				case WSATRY_AGAIN:
					printf("ERROR: No host found or the server failed. Please try again.");
					break;
				case WSAENETDOWN:
					printf("ERROR: The network subsystem has failed. Dead network has been encountered.");
					break;
				case WSAENOBUFS:
					printf("ERROR: Insufficient buffer space is available or queue is full");
					break;
				case WSANO_RECOVERY:
					printf("ERROR: Nonrecoverable error during database lookup.");
					break;
				case WSANO_DATA:
					printf("ERROR: Valid name but no data record of requested type.");
					break;
				default:
					printf("ERROR: Unknown error.");
					break;
			}
			break;
		}
		hdc = GetDC(hwnd);
		InvalidateRect(hwnd, NULL, true);
		break;
	case PORT_SERVICE_NAME:
		//error checking
		if (HIWORD(lParam) != 0)
		{
			switch(HIWORD(lParam))
			{
				case WSAHOST_NOT_FOUND:
					printf("ERROR: Host was not found. Try Again");
					break;
				case WSATRY_AGAIN:
					printf("ERROR: No host found or the server failed. Please try again.");
					break;
				case WSAENETDOWN:
					printf("ERROR: The network subsystem has failed. Dead network has been encountered.");
					break;
				case WSAENOBUFS:
					printf("ERROR: Insufficient buffer space is available or queue is full");
					break;
				case WSANO_RECOVERY:
					printf("ERROR: Nonrecoverable error during database lookup.");
					break;
				case WSANO_DATA:
					printf("ERROR: Valid name but no data record of requested type.");
					break;
				default:
					printf("ERROR: Unknown error.");
					break;
			}
			break;
		}
		hdc = GetDC(hwnd);
		sprintf_s(varNeeded->tmp, 3, "%d", ntohs(varNeeded->sv->s_port));
		InvalidateRect(hwnd, NULL, false);
		TextOut(hdc, 0, 0, varNeeded->tmp, LENGTH_LINE);
		TextOut(hdc, 0, 20, varNeeded->sv->s_proto, strlen(varNeeded->sv->s_proto));
		TextOut(hdc, 0, 40, varNeeded->sv->s_name, strlen(varNeeded->sv->s_name));
		break;
	default:
		return DefFrameProc (hwnd, hwndClient, Message, wParam, lParam);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Window_OnPaint
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: void Window_OnPaint(HWND hwnd)
--				HWND hwnd: Handle to the window
--
-- RETURNS: 
--
-- NOTES:
-- This function repaints the window. Only the IP/Host name is properly updated.
------------------------------------------------------------------------------------------------------------------*/
void Window_OnPaint(HWND hwnd){
	HDC hdc;
	PAINTSTRUCT paint;
	RECT lpRect;
	char **p;
	int i = 0;
	VariablesNeeded* varNeeded = (VariablesNeeded*) GetWindowLongPtr(hwnd, 0);
	hdc = BeginPaint(hwnd, &paint);
	
	for (p = varNeeded->hp->h_addr_list; *p != 0; p++, i++)
	{
   		struct in_addr in;
		char **q;
		memcpy(&in.s_addr, *p, sizeof (in.s_addr));
		TextOut(hdc, 0, 60+(120*i), inet_ntoa(in), strlen(inet_ntoa(in)));
		TextOut(hdc, 0, 80+120*i, varNeeded->hp->h_name, strlen(varNeeded->hp->h_name));
		for (q = varNeeded->hp->h_aliases; *q != 0; q++, i++)
			TextOut(hdc, 0, 100+(i*20), *q, strlen(*q));
	}
	
	EndPaint(hwnd, &paint);

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Window_OnCreate
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL Window_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
--				HWND hwnd: Handle to the window
--				LPCREATESTRUCT lpCreateStruct: Create struct
--
-- RETURNS: Returns true if successful. 
--
-- NOTES:
-- This function deals with the window creation. Upon creation, a Winsock sessions will be opened.
------------------------------------------------------------------------------------------------------------------*/
BOOL Window_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct){
	WORD wVersionRequested = MAKEWORD(2,2);   
	WSADATA wsaData;  
	if(WSAStartup(wVersionRequested, &wsaData)!= 0) // Open up a Winsock v2.2 session
		return false;	
	return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Window_OnDestroy
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: void Window_OnDestroy (HWND hwnd)
--				HWND hwnd: Handle to the window
--
-- RETURNS: - 
--
-- NOTES:
-- This function will do socket cleanup and shut down the program.
------------------------------------------------------------------------------------------------------------------*/
void Window_OnDestroy (HWND hwnd){
	WSACleanup();
    PostQuitMessage(0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Window_OnCommand
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: void Window_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
--				HWND hwnd: Handle to the window
--				UINT Message: The message
--				HWND hwndCtl: Handle for the control.
--				UINT codeNotify: unsigned int.
--
-- RETURNS: -
--
-- NOTES:
-- This function will process the information from the Dialog Boxes and will either:
--		- Get the Host Name by an IP Address
--		- Get the IP Address by the Host Name
--		- Get a Port Number by the Service Name and Protocol Type.
--		- Get a Service Name by the Port Number and Protocol Type.
-- After the desired type is requested, the information is stored in the VariablesNeeded struct, where the information
-- can be accessed and displayed to the screen.
------------------------------------------------------------------------------------------------------------------*/
void Window_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify){
	VariablesNeeded* varNeeded = (VariablesNeeded*) GetWindowLongPtr(hwnd, 0); 
	struct	in_addr my_addr, *addr_p;
    switch(id){
		case IDM_IP:
			
			if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, IPDlgProc, (LPARAM)varNeeded))
                    InvalidateRect (hwnd, NULL, TRUE) ;
			if (varNeeded->exitValue==false){
				break;
			}
			addr_p = (struct in_addr*)malloc(sizeof(struct in_addr)); 
			addr_p->s_addr = varNeeded->ip_addr;
			if(WSAAsyncGetHostByAddr(hwnd, IP_HOST_MSG, (char*)addr_p, 4, PF_INET, (char*)varNeeded->hp ,MAXGETHOSTSTRUCT)==0)
			{
				printf("Unable to get Host by the IP Address.");
				exit(1);
			}
			break;
		case IDM_HOST:
			if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, HostDlgProc, (LPARAM)varNeeded))
                    InvalidateRect (hwnd, NULL, TRUE) ;
			if (varNeeded->exitValue==false){
				break;
			}
			if(WSAAsyncGetHostByName(hwnd, IP_HOST_MSG, varNeeded->hostName, (char*)varNeeded->hp, MAXGETHOSTSTRUCT) == 0)
			{
				printf("Unable to get the IP Address by the Host Name.");
				exit(1);
			}
			break;
		case IDM_PORT_NUMBER:
			if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PortNumDlgProc, (LPARAM)varNeeded))
                    InvalidateRect (hwnd, NULL, TRUE) ;
			if (varNeeded->exitValue==false){
				break;
			}
			if(WSAAsyncGetServByName(hwnd, PORT_SERVICE_NAME, varNeeded->serviceName, varNeeded->protocolType, (char*)varNeeded->sv, MAXGETHOSTSTRUCT)==0)
			{	
				printf("Unable to get Port Number by the Service Name and the Protocol Type.");
				exit(1);
			}
			break;
		case IDM_SERVICE_NAME:
			if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG4), hwnd, ServiceNameDlgProc, (LPARAM)varNeeded))
                    InvalidateRect (hwnd, NULL, TRUE) ;
			if (varNeeded->exitValue==false){
				break;
			}
			if(WSAAsyncGetServByPort(hwnd, PORT_SERVICE_NAME, htons(varNeeded->s_port), varNeeded->protocolType, (char*)varNeeded->sv, MAXGETHOSTSTRUCT)==0)
			{	
				printf("Unable to get Service Name by Port Number and Protocol Type");
				exit(1);
			}
			break;
		case IDM_EXIT:        
			Window_OnDestroy(hwnd);
			break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: IPDlgProc
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL CALLBACK IPDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--				HWND hDlg: Handle to the Dialog Box
--				UINT Message: The message
--				WPARAM wParam: Parameter
--				LPARAM lParam: Parameter
--
-- RETURNS: 
--
-- NOTES:
-- This function will open up the Dialog Box for getting a Host Name from an IP Address. The user will enter an IP
-- address and will press OK to confirm. The information will be stored in the VariablesNeeded Struct.
------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK IPDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static VariablesNeeded* varNeeded;
	switch (message)
    {
		case WM_INITDIALOG:
			varNeeded = (VariablesNeeded*) lParam;
			break;
		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					varNeeded->exitValue = true;
					if(!GetDlgItemText(hDlg, DIALOG_RESOLVE_TEXT, varNeeded->IPAddress, 128)){
						printf("Unable to get the IP Address");
						return -1;
					}
					if((varNeeded->ip_addr = inet_addr(varNeeded->IPAddress))==INADDR_NONE)
					{
						printf("IP Address is not a legitimate Internet address");
						return -1;
					}
					EndDialog(hDlg,0);
					break;
				case IDCANCEL:
				case WM_CLOSE: 
					varNeeded->exitValue = false;
					EndDialog(hDlg,0);
					return false;
			}
		default:
			return false;
	}
	return false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: HostDlgProc
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL CALLBACK HostDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--				HWND hDlg: Handle to the Dialog Box
--				UINT Message: The message
--				WPARAM wParam: Parameter
--				LPARAM lParam: Parameter
--
-- RETURNS: 
--
-- NOTES:
-- This function will open up the Dialog Box for getting an IP Address from a Host Name. The user will enter a Host Name
-- and will press OK to confirm. The information will be stored in the VariablesNeeded Struct.
------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK HostDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static VariablesNeeded* varNeeded;
	switch (message)
    {
		case WM_INITDIALOG:
			varNeeded = (VariablesNeeded*) lParam;
			break;
		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					varNeeded->exitValue = true;
					if(!GetDlgItemText(hDlg, IDC_HOST, varNeeded->hostName, 128)){
						printf("Unable to get the Host Name");
						return -1;
					}
					EndDialog(hDlg,0);
					break;
				case IDCANCEL:
				case WM_CLOSE: 
					EndDialog(hDlg,0);
					varNeeded->exitValue = false;
					return false;
			}
		default:
			return false;
	}
	return false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PortNumDlgProc
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL CALLBACK PortNumDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--				HWND hDlg: Handle to the Dialog Box
--				UINT Message: The message
--				WPARAM wParam: Parameter
--				LPARAM lParam: Parameter
--
-- RETURNS: 
--
-- NOTES:
-- This function will open up the Dialog Box for getting the Port Number. The user will enter both the Service Name
-- and the protocol type and will press OK to confirm. The information will be stored in the VariablesNeeded Struct.
------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK PortNumDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static VariablesNeeded* varNeeded;
	switch (message)
    {
		case WM_INITDIALOG:
			varNeeded = (VariablesNeeded*) lParam;
			break;
		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					varNeeded->exitValue = true;
					if(!GetDlgItemText(hDlg, IDC_SERVICE_NAME, varNeeded->serviceName, 128)){
						printf("Unable to get the Service Name");
						return -1;
					}
					if(!GetDlgItemText(hDlg, IDC_PROTOCOL, varNeeded->protocolType, 128)){
						printf("Unable to get the Protocol Type");
						return -1;
					}
					EndDialog(hDlg,0);
					break;
				case IDCANCEL:
				case WM_CLOSE: 
					varNeeded->exitValue = false;
					EndDialog(hDlg,0);
					return false;
			}
		default:
			return false;
	}
	return false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ServiceNameDlgProc
--
-- DATE: January 13, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: BOOL CALLBACK ServiceNameDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--				HWND hDlg: Handle to the Dialog Box
--				UINT Message: The message
--				WPARAM wParam: Parameter
--				LPARAM lParam: Parameter
--
-- RETURNS: 
--
-- NOTES:
-- This function will open up the Dialog Box for getting the service name. The user will enter both the Port Number
-- and the protocol type and will press OK to confirm. The information will be stored in the VariablesNeeded Struct.
------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK ServiceNameDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static VariablesNeeded* varNeeded;
	int portNumber = 0;
	switch (message)
    {
		case WM_INITDIALOG:
			varNeeded = (VariablesNeeded*) lParam;
			break;
		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					varNeeded->exitValue = true;
					if(!GetDlgItemText(hDlg, IDC_PORT_NUMBER, varNeeded->portNumber, 128)){
						printf("Unable to get the Port Number");
						return -1;
					}
					varNeeded->s_port = atoi(varNeeded->portNumber);
					if(!GetDlgItemText(hDlg, IDC_PROTOCOL, varNeeded->protocolType, 128)){
						printf("Unable to get the Protocol Type");
						return -1;
					}
					EndDialog(hDlg,0);
					break;
				case IDCANCEL:
				case WM_CLOSE: 
					varNeeded->exitValue = false;
					EndDialog(hDlg,0);
					return false;
			}
		default:
			return false;
	}
	return false;
}
