#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tchar.h>


int read_proc_mem(_TCHAR *windowname,void *address,unsigned char *data,int len)
{
	HWND hwnd;
	HANDLE phandle;
	DWORD pid;
	SIZE_T bread;
	int result=FALSE;
	hwnd=FindWindow(0,windowname);
	if(hwnd==0){
		printf("error:cant find window\n");
		return result;
	}
	GetWindowThreadProcessId(hwnd,&pid);
	phandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(phandle==0){
		printf("error:cant open process!\n");
		return result;
	}
	result=ReadProcessMemory(phandle,address,data,len,&bread);
	CloseHandle(phandle);
	return result;
}
int write_proc_mem(_TCHAR *windowname,void *address,unsigned char *data,int len)
{
	HWND hwnd;
	HANDLE phandle;
	DWORD pid;
	SIZE_T bread;
	int result=FALSE;
	hwnd=FindWindow(0,windowname);
	if(hwnd==0){
		printf("Error: Cannot Window!\n");
		return result;
	}
	GetWindowThreadProcessId(hwnd,&pid);
	phandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(phandle==0){
		printf("Error: Cannot Process!\n");
		return result;
	}
	result=WriteProcessMemory(phandle,address,data,len,&bread);
	CloseHandle(phandle);
	return result;
}
int alloc_proc_mem(_TCHAR *windowname,void **address,SIZE_T len)
{
	HWND hwnd;
	HANDLE phandle;
	DWORD pid;
	SIZE_T bread;
	int result=FALSE;
	hwnd=FindWindow(0,windowname);
	if(hwnd==0){
		printf("Error: Cannot Window!\n");
		return result;
	}
	GetWindowThreadProcessId(hwnd,&pid);
	phandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(phandle==0){
		printf("Error: Cannot Process!\n");
		return result;
	}
	*address=VirtualAllocEx(phandle,NULL,len,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	if(*address!=0)
		result=TRUE;
	CloseHandle(phandle);
	return result;
}
int test()
{
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char data[100];
	const _TCHAR *procname=L"Process Monitor - Sysinternals: www.sysinternals.com";
	printf("test\n");
	memset(data,0,sizeof(data));
	memset(data,'A',sizeof(data)/2);
	if(read_proc_mem(procname,(void*)0x410000,data,4)){
		int i;
		printf("read successfull\n");
		for(i=0;i<4;i++){
			int a=data[i];
			if(a<' ' || a>0x7F)
				a='.';
			printf("%c",a);
		}
		printf("\n");
		for(i=0;i<4;i++){
			int a=data[i];
			printf("%02X ",a);
		}
		printf("\n");

	}
	{
		void *address=0;
		//alloc_proc_mem(procname,&address,1000);
		printf("%016I64X\n",address);
	}
	getch();
	return 0;
}

