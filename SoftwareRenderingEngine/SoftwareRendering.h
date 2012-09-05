#include "Mathematics.h"
#include "SoftwareDevice.h"
#include "Mathematics.h"

#pragma once


enum JSS_DEVICE_TYPE {SOFTWARE = 0};

namespace JssEngine
{
	IDevice* CreateDevice(Vector2i ScreenSize, JSS_DEVICE_TYPE type);
}