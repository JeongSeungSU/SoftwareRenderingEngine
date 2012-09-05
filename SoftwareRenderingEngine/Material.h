#pragma once
#include "Texture.h"


namespace JssEngine
{
	class Material
	{
	private:
		Texture*		m_pTexture;
		//이제 안에 것들을 넣자
	public:
		Material(Texture* pTexture);
		~Material(void);


		const Texture*	GetTexture(){return m_pTexture;}
	};

}