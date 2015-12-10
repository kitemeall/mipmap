
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>	//you need SDL_image to compile this
#include <GL/gl.h>
#include <GL/glu.h>	//the camera class from earlier
#include <vector>
#include "camera.h"

camera cam;

unsigned int loadTexture(const char* filename,bool generate=false)
{
	unsigned int num;
	glGenTextures(1,&num);
	SDL_Surface* img=IMG_Load(filename);
	if(img==NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);
	if(img2==NULL)
	{
        std::cout << "surface was not loaded" << std::endl;
		return -1;		
	}
	glBindTexture(GL_TEXTURE_2D,num);		
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	

	if(generate)
		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img2->w,img2->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
	
	if(generate)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	else
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		

		
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;
}

unsigned int mipMap,noMipMap,current;
void init()
{
	glClearColor(0,0,0,1);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(50,640.0/480.0,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
    current=mipMap=loadTexture("img/chess.jpg",true);
    noMipMap=loadTexture("img/chess.jpg",false);
}

void display()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	cam.Control();
	cam.UpdateCamera();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,current);
	glBegin(GL_QUADS);

        glTexCoord2f(0,50);
        glVertex3f(-100,-3,100);

		glTexCoord2f(0,0);        
        glVertex3f(-100,-3,-100);

        glTexCoord2f(50,0);
        glVertex3f(100,-3,-100);

        glTexCoord2f(50,50);
        glVertex3f(100,-3,100);

	glEnd();
	
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640,480,32,SDL_OPENGL);
	Uint32 start;
	SDL_Event event;
	bool running=true;
	init();
	bool b=false;
	while(running)
	{
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running=false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running=false;
							break;
						case SDLK_RETURN:
                            if(current==mipMap)
                                current=noMipMap;
							else
                                current=mipMap;
							break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					cam.mouseIn(true);
					break;
					
			}
		}
		display();
		SDL_GL_SwapBuffers();
		if(1000.0/30>SDL_GetTicks()-start)
			SDL_Delay(1000.0/30-(SDL_GetTicks()-start));
	}
	SDL_Quit();
}
