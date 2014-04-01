#ifndef RESOURCE_H
#define RESOURCE_H

#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#define IDM_IP						40000
#define IDM_HOST					40001
#define IDM_PORT_NUMBER				40002
#define IDM_SERVICE_NAME			40003
#define IDM_EXIT					40004
#define IDC_HOST					40005
#define IDC_SERVICE_NAME			40006
#define IDC_PROTOCOL				40007
#define IDC_PORT_NUMBER				40008
#define IDC_STATIC					40009
#define IDD_DIALOG1                 102
#define IDD_DIALOG2                 103
#define IDD_DIALOG3                 104
#define IDD_DIALOG4                 105
#define IDC_EDIT1                   1002
#define IDC_EDIT2                   1003
#define IDC_EDIT3                   1004
#define IDC_EDIT4                   1005
#define DIALOG_RESOLVE_TEXT			1006

#define IP_HOST_MSG					21000
#define HOST_IP_MSG					21001
#define PORT_SERVICE_NAME			21002

#define LENGTH_LINE					50

#define UNTITLED TEXT ("(untitled)")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL Register(HINSTANCE);
HWND Create(HINSTANCE, int);
BOOL Window_OnCreate(HWND, LPCREATESTRUCT);
void Window_OnCommand (HWND, int, HWND, UINT);
void Window_OnPaint(HWND);
void Window_OnDestroy (HWND);

BOOL CALLBACK IPDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK HostDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PortNumDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ServiceNameDlgProc (HWND, UINT, WPARAM, LPARAM);



typedef struct variablesNeeded{
	struct servent *sv;
	struct hostent *hp;
	int s_port;
	int ip_addr;
	TCHAR protocolType[64];
	TCHAR serviceName[64];
	TCHAR portNumber[64];
	TCHAR hostName[128];
	TCHAR IPAddress[128];
	BOOL exitValue;
	char tmp[50];				//Temp for the aliases
} VariablesNeeded;


#endif
