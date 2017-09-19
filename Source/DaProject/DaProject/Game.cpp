#define SAFE_DELETE(p) { if (p) { delete p; (p) = NULL; } }
#define GCC_ERROR(str) do { (void) sizeof(str); } while(0)
#define GCC_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)

#include "Game.h"
#include "Windows.h"
#include "direct.h"
#include <iostream>


bool IsOnlyInstance(LPCTSTR gameTitle)
{
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS) 
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd)
		{
			// An instance of your game is already running.
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded) 
{
	// Check for enough free disk space on the current disk.
	int const drive = _getdrive();
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);

	unsigned __int64 const neededClusters = diskSpaceNeeded / (diskfree.sectors_per_cluster*diskfree.bytes_per_sector);

	if (diskfree.avail_clusters < neededClusters) 
	{
		// if you get here you don’t have enough disk space!
		GCC_ERROR("CheckStorage Failure : Not enough physical storage.");
		return false;
	}
	return true;
}

bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);

	if (status.ullTotalPhys < physicalRAMNeeded)
	{
		/* you don’t have enough physical memory. Tell the
		player to go get a real computer and give this one to
		his mother. */
		GCC_ERROR("CheckMemory Failure: Not enough physical memory.");
		return false;
	}

	//check for enough free memory
	if (status.ullAvailVirtual < virtualRAMNeeded)
	{
		//you don't have enough virtual memory available.
		//Tell the player to shut down the copy of Visual Studio running in the background.
		GCC_ERROR("CheckMemory Failure: Not enough virtual memory.");
		return false;
	}

	char *buff = GCC_NEW char[virtualRAMNeeded];

	if (buff)
	{
		delete[] buff;
	}
	else
	{
		// even though there is enough memory, it isn’t available in one block, which can be critical for games that manage their own memory
		GCC_ERROR("CheckMemory Failure: Not enough contiguous memory.");
		return false;
	}
}

DWORD ReadCPUSpeed() 
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) 
	{
		// query the key:
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}
	return dwMHz;
}


void Game::initGame() 
{
	//Lab 2

	//Step1
	if (IsOnlyInstance(gameTitle.c_str()))
	{
		std::cout << "Game running only instance." << std::endl;
	}
	else
	{
		std::cout << "Game instance already running." << std::endl;
	}

	//Step2
	if (CheckStorage(314572800))
	{
		std::cout << "Sufficient storage available." << std::endl;
	}
	else 
	{
		std::cout << "Not enough storage available." << std::endl;
	}

	//Step3
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	std::cout << "Available RAM : " << status.ullAvailPhys / 1000000000 << " GB." << std::endl;
	std::cout << "Available Virtual Memory : " << status.ullAvailVirtual / 1000000000 << " GB." << std::endl;

	//Step4
	std::cout << "CPU Speed : " << ReadCPUSpeed() << " MHz." << std::endl;
	char archi[1024];
	DWORD BufSize = 1024;
	HKEY hKey;
	DWORD type = REG_SZ;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ, &hKey);
	RegQueryValueEx(hKey, "PROCESSOR_ARCHITECTURE", NULL, &type, (LPBYTE)&archi, &BufSize);
	std::cout << "Architecture : " << archi << std::endl;
}