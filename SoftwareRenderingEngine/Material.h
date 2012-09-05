#pragma once
#include "Texture.h"


namespace JssEngine
{
	class Material
	{
	private:
		Texture*		m_pTexture;
		//���� �ȿ� �͵��� ����
	public:
		Material(Texture* pTexture);
		~Material(void);


		const Texture*	GetTexture(){return m_pTexture;}
	};

}