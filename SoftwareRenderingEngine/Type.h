/*!
 * @file      Type.h
 * @brief     타입에 대한 정의들...
 * @author    JSS(E-Mail : sjdmldi@naver.com)
 * @date      2012-05-01 
 * @warning   
 */
#pragma once

namespace JssEngine
{
	enum RENDERSTATE
	{
		JSS_FILLMODE,
		JSS_LIGHT,
		JSS_CULLMODE,
		JSS_ZBUFFER,
		JSS_TEXTURE
	};

	enum FILLMODE
	{
		JSS_FILL_POINT,
		JSS_FILL_WIRE,
		JSS_FILL_SOLID
	};

	enum CULLMODE
	{
		JSS_CULL_CCW,
		JSS_CULL_CW,
		JSS_CULL_NONE
	};
	enum TYPE {JSS_INTTPYE,JSS_FLOATTPYE,JSS_DOUBLETPYE,JSS_UINTTPYE,JSS_USHORTTPYE,JSS_SHORTTPYE};
	enum TRANSFORM_TYPE{JSS_TRANS_WORLD, JSS_TRANS_VIEW, JSS_TRANS_PROJECTION};
	enum PRIMITE_TYPE{JSS_PRIMITE_TRIANGLE,JSS_PRIMITE_QUAD,JSS_PRIMITE_STRIP};



#define JSSFVF_XYZ		0x001

#define JSSFVF_NORMAL	0x010

	//멀티 텍스쳐를 위한 준비...
#define JSSFVF_TEX0		0x100
// #define JSSFVF_TEX1		0x200
// #define JSSFVF_TEX2		0x300
// #define JSSFVF_TEX3		0x400
// #define JSSFVF_TEX4		0x500
// #define JSSFVF_TEX5		0x600
// #define JSSFVF_TEX6		0x700
// #define JSSFVF_TEX7		0x800

}