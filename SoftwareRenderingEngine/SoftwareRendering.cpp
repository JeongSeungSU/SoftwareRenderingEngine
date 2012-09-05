#include "SoftwareRendering.h"




namespace JssEngine
{
	IDevice* CreateDevice(Vector2i ScreenSize, JSS_DEVICE_TYPE type)
	{
		switch (type)
		{
			case SOFTWARE:
				return new SoftwareDevice(ScreenSize);

			default:
				return NULL;
		}
	}
}