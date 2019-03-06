#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 1) {
		cout << "you should specify console free event name!" << endl;
		return 1;
	}
	LPSTR eventName = argv[0];
	HANDLE event = CreateEvent(NULL, FALSE, TRUE, eventName);

	DWORD pid = GetCurrentProcessId();
	string msg = "Hello! I'm a process with number ";
	msg += to_string((unsigned long)pid);

	for (;;) {
		WaitForSingleObject(event, INFINITE);
		Sleep(500);
		for (auto& c : msg) {
			cout << c;
			Sleep(40);
		}
		cout << endl;
		SetEvent(event);
	}

	return 0;
}