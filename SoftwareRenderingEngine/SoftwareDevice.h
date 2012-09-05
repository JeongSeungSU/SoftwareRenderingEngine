#pragma once

#include "idevice.h"
#include <windows.h>
#include "Mathematics.h"
#include "Buffer.h"
#include "SoftwareVideoManager.h"

namespace JssEngine
{
	struct WMessage
	{
		UINT	iMessage;
		WPARAM	wParam;
		LPARAM	lParam;
	};


	class SoftwareDevice : 	public IDevice
	{
	private:
		///������//////
		HWND		m_hWnd;
		MSG			Message;
		
		HINSTANCE	m_hInst;
		WMessage	m_NowMessage;
		///������//////

		Vector2i	m_ScreenSize;
		Vector2i	m_StartScreen;



		bool		m_bExit;
		bool		m_bFullScreen;

		


	private:
	
		void	InitDevice();
		void	InitWindow();
	protected:

	public:
		SoftwareDevice(Vector2i ScreenSize);
		~SoftwareDevice(void);
	public:
		//�⺻ �Լ�//
		virtual bool Run();
		virtual void Init();


		

		
		bool GetMessageListener(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

		
	};
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);