#pragma once

#include "ApplyModule.h"

namespace JssEngine
{
	class LightModule
	{
	private:
		const ApplyModule* m_pApplyModule;
	public:
		LightModule();
		~LightModule(void);
	};

}