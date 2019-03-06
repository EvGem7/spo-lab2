#include <iostream>
#include <stack>
#include <Windows.h>
#include <string>
#include <conio.h>

using namespace std;

const LPCSTR CHILD_PATH = "..\\Debug\\child.exe";

bool createChild(PROCESS_INFORMATION* processInfo, LPSTR eventName);
void killChild(PROCESS_INFORMATION& processInfo);

int main(int argc, char** argv) {
	stack<PROCESS_INFORMATION> childs;

	LPSTR eventName = const_cast<char*>("spo-lab2-console-free-event");
	HANDLE consoleFreeEvent = CreateEvent(NULL, FALSE, TRUE, eventName);

	char input;
	bool isExit = false;
	while (!isExit) {
		input = _getch();
		switch (input) {
			case '+':
				PROCESS_INFORMATION processInfo;
				if (createChild(&processInfo, eventName)) {
					childs.push(processInfo);
				}
				break;
			case '-':
				WaitForSingleObject(consoleFreeEvent, INFINITE);
				if (!childs.empty()) {
					killChild(childs.top());
					childs.pop();
				} else {
					cout << "What is dead may never die" << endl;
				}
				SetEvent(consoleFreeEvent);
				break;
			case 'q':
				isExit = true;
				while (!childs.empty()) {
					killChild(childs.top());
					childs.pop();
				}
				break;
		}
	}

	CloseHandle(consoleFreeEvent);
	return 0;
}

void killChild(PROCESS_INFORMATION& child) {
	TerminateProcess(child.hProcess, 0);
	CloseHandle(child.hProcess);
	CloseHandle(child.hThread);
}

bool createChild(PROCESS_INFORMATION* processInfo, LPSTR eventName) {
	STARTUPINFO startUpInfo;
	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	/*startUpInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);*/

	if (!CreateProcess(
		CHILD_PATH,
		eventName,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&startUpInfo,
		processInfo
	)) {
		std::cout << "Cannot create process!" << std::endl;
		return false;
	}
	return true;
}