#include "SoftwareRendering.h"
#include "AsePaser.h"
#ifdef DEBUGDATA
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__ )
#endif



using namespace JssEngine;

float size = 2.f;
float vt[108] = 
{
	-size,size,size, //1
	size,size,size, //2
	-size,size,-size, //3

	size,size,size, //2
	size,size,-size, //4
	-size,size,-size, //3

	size,size,size, //2
	size,-size,-size, //8
	size,-size,size, //6

	size,size,size, //2
	size,-size,-size, //8
	size,size,-size, //4

	-size,size,-size, //3
	size,size,-size, //4
	-size,-size,-size, //7

	size,size,-size, //4
	-size,-size,-size, //7
	size,-size,-size, //8

	-size,size,size, //1
	-size,-size,-size, //7
	-size,-size,size, //5

	-size,size,size, //1
	-size,size,-size, //3
	-size,-size,-size, //7

	-size,size,size, //1
	size,size,size, //2
	-size,-size,size, //5

	size,size,size, //2
	-size,-size,size, //5
	size,-size,size, //6

	-size,-size,size, //5
	size,-size,size, //6
	size,-size,-size, //8

	-size,-size,-size, //7
	size,-size,-size, //8
	-size,-size,size //5
};

//  float vert[] = 
//  {
//  	-size,0,0,0.0f,1.0f,
//  	0,size,0,0.5f,0.0f,
//  	size,0,0,1.0f,1.0f
//  };

 float vert[] = 
 { 
 	-size,-size,0,    0,0,
 	-size,size,0,     0,1.0f,
 	size,size,0,	  1.0f,1.0f,

	-size,-size,0,	  0.0f,0.0f,
	size,size,0,	  1.0f,1.0f,
	size,-size,0,	  1.0f,0.0f
 };

 float vert2[] = 
 { 
	 -size,0,1.0f,   0,0.0f,
	 0,size,1.0f,    0.5f,1.0f,
	 size,0.0f,1.0f, 1.0f,0.0f
 };



void main()
{
	#ifdef DEBUGDATA
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(484); 
	#endif

	IDevice* device = CreateDevice(Vector2i(800,600),SOFTWARE);
	
	Matrix16f rtx;
	Matrix16f rtxy;
	Matrix16f scale;
	Matrix16f trans;
	ZeroMemory(&rtx,sizeof(rtx));
	float i = 0;
	float j = 0;
	VertexBuffer* pBuffer, *pBuffer2;
	JssEngine::DWORD FVF = JSSFVF_XYZ | JSSFVF_TEX0;
	float* pVertex;
	Vector3f v(1.f,1.f,1.f);
	float a= 1.0f;
	float b = 0.001f;

// 	device->GetVideoManager()->CreateVertexBuffer(3*3*12*sizeof(float),FVF,&pBuffer);
// 	pBuffer->Lock(0,108,(void**)&pVertex);
// 	memcpy(pVertex,&vt,sizeof(vt));
// 	pBuffer->Unlock();

	device->GetVideoManager()->CreateVertexBuffer(30*sizeof(float),FVF,&pBuffer);
	pBuffer->Lock(0,30,(void**)&pVertex);
	memcpy(pVertex,&vert,sizeof(vert));
	pBuffer->Unlock();
//	device->GetVideoManager()->CreateTexture(0,"media/texture.bmp");

	device->GetVideoManager()->CreateVertexBuffer(15*sizeof(float),FVF,&pBuffer2);
	pBuffer2->Lock(0,15,(void**)&pVertex);
	memcpy(pVertex,&vert2,sizeof(vert2));
	pBuffer2->Unlock();
//	device->GetVideoManager()->CreateTexture(1,"media/texture2.bmp");
	
	
	bool update = true;

	device->GetVideoManager()->SetRenderState(JSS_TEXTURE,true);
	device->GetVideoManager()->SetRenderState(JSS_CULLMODE,JSS_CULL_CCW);
	device->GetVideoManager()->SetRenderState(JSS_ZBUFFER,true);
	//device->GetVideoManager()->SetRenderState(JSS_FILLMODE,JSS_FILL_WIRE);

	CAsePaser parser;
	CContainer3D container;
	CAnimationControl ani;
	parser.ReadASE("woman_01_all.ASE",&container,&ani,device);
	//parser.ReadASE("box.ASE",&container,&ani,device);


	while(device->Run())
	{
		if(i > 360)
			i =0; 
		if( j> 360)
			j=0;
		if(a > 3.f)
			b *= -1;
		if(a < 1.0f)
			b *= -1;

		if(update)
		{
			
			i+= 1.0f;
			j+= 0.1f;
		}
		
		
		
 		device->GetVideoManager()->BeginDraw();
 	
  		
  		Scale(scale,10.0f*v);
		Translate(trans,Vector3f(-1,-5,0));
  		RotationX(rtx,90);
  		RotationY(rtxy,i);
		
		
  		
  		rtx = scale*rtx*rtxy*trans;
  
  		device->GetVideoManager()->SetTransform(JSS_TRANS_WORLD,&rtx);
  
		Vector3f Up(0.0f,1.0f,0.0f);
		Vector3f dir(0.0f,0.0f,-1.0f);
		Vector3f side(1.0f,0.0f,0.0f);
		Vector3f position(0.0f,0.0f,50.0f);

  		device->GetVideoManager()->TransformView(Up,dir,side,position);
  
  		device->GetVideoManager()->TransformProjection(800.f/600.f,1.0f,100.f,20);
  
 
 		//device->GetVideoManager()->LineDrawing(Vector2i(600,10),Vector2i(800,600));

		container.draw();

		
 

// 		device->GetVideoManager()->BindTexture(1);
// 
//  		device->GetVideoManager()->SetVertexBuffer(pBuffer);
// 		device->GetVideoManager()->SetFVF(FVF);
// 
// 		device->GetVideoManager()->DrawPrimited(JSS_PRIMITE_TRIANGLE,0,6);
// 
// 
// 
// 		device->GetVideoManager()->BindTexture(0);
// 
// 		device->GetVideoManager()->SetVertexBuffer(pBuffer2);
// 		device->GetVideoManager()->SetFVF(FVF);
// 
// 		device->GetVideoManager()->DrawPrimited(JSS_PRIMITE_TRIANGLE,0,3);
// 		
  		device->GetVideoManager()->EndDraw();
	}
	pBuffer->ReleaseBuffer();
	pBuffer2->ReleaseBuffer();
	device->Release();
	return;
}
