#pragma once

#include "Mathematics.h"

namespace JssEngine
{
	struct ApplyModule
	{
		DWORD			m_FillMode;
		DWORD			m_Light;
		DWORD			m_CULLMODE;

		bool			m_Zbuffering;
		bool			m_TextrueApply;
		bool			m_bCulling;
		bool			m_bLighting;
	};
}