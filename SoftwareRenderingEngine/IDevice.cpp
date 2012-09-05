#include "IDevice.h"
#include "Video.h"


namespace JssEngine
{
	IDevice::IDevice(void)
	{
		m_pVideoManager = NULL;
		m_Continue		= true;
	}

	IDevice::~IDevice(void)
	{
		delete m_pVideoManager;
	}
	void IDevice::Release()
	{
		delete this;
	}

}