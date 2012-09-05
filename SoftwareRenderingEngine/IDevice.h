#pragma once



namespace JssEngine
{
	class VideoManager;

	class IDevice
	{

	protected:
		VideoManager*	m_pVideoManager;
		bool			m_Continue;
	public:
		IDevice(void);
		virtual ~IDevice(void);

	public:
		virtual bool Run() = 0;
		virtual void Release();
		virtual void Init() = 0;

		VideoManager*		GetVideoManager(){return m_pVideoManager;}
	};

}