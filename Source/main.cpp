#include <windows.h>
#include "JohanEngine11\Renderer.h"
#include "JohanEngine11\Interface.h"
#include "JohanEngine11\Resource.h"

void CreateEngine(HWND hwnd) {
	interface = new Interface(hwnd);
	renderer = new Renderer(hwnd);
}

void DeleteEngine() {
	delete renderer;
	delete interface;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static bool IsResizing = false;
	switch(Message) {
		case WM_ENTERSIZEMOVE: {
			IsResizing = true;
			break;
		}
		case WM_EXITSIZEMOVE: {
			IsResizing = false;
			if(renderer) {
				renderer->OnResize();
			}
			break;
		}
		case WM_SIZE: {
			if(renderer && !IsResizing) {
				renderer->OnResize();
			}
			break;
		}
		case WM_MENUCHAR: {
			// Don't beep when using Alt+Enter
			return MAKELRESULT(0, MNC_CLOSE);
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default: {
			return DefWindowProc(hwnd, Message, wParam, lParam);
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL,"A");
	wc.hIconSm		 = LoadIcon(NULL,"A");

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","JohanEngine11 Demo",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	CreateEngine(hwnd);
	
	// Handle user input. If done, render a frame. Goto 1
	while(msg.message != WM_QUIT) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	//	if(!renderer->paused) {
	//		renderer->Begin(false);
	//		renderer->DrawScene(scene);
		renderer->RenderFrame();
	//		renderer->End();
	//	} else {
	//		Sleep(100);
	//	}
	}
	
	DeleteEngine();
	
	return msg.wParam;
}
