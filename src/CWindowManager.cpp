#include"CWindowManager.hpp"

#include<iostream>
#include<AntTweakBar.h>

namespace NDormon{

	CWindowManager::CWindowManager(
			unsigned Width,
			unsigned Height,
			bool FullScreen,
			void(*Idle)(),
			void(*Mouse)(),
			bool UseAntTweakBar){
		this->MapKeyDown.clear();
		this->MapKeyOffOn.clear();
		this->Idle=Idle;//set idle function
		this->Mouse=Mouse;//set mouse function
		this->WindowSize[0]=Width;//width of window
		this->WindowSize[1]=Height;//height of window
		this->IsFullScreen=FullScreen;//set fullscreen flag
		this->UseAntTweakBar=UseAntTweakBar;

		if(this->UseAntTweakBar){
			TwInit(TW_OPENGL,NULL);
			TwWindowSize(this->WindowSize[0],this->WindowSize[1]);
		}

		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);//initialise video
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
		if(this->IsFullScreen)//is fullscreen?
			SDL_SetVideoMode(//set video
					this->WindowSize[0],//width
					this->WindowSize[1],//height
					24,//bit depth
					SDL_HWSURFACE|//hardware surface
					SDL_DOUBLEBUF|//double buffering
					SDL_OPENGL|//OpenGL
					SDL_FULLSCREEN);//fullscreen
		else
			SDL_SetVideoMode(//set video
					this->WindowSize[0],//width
					this->WindowSize[1],//height
					24,//bit depth
					SDL_HWSURFACE|//hardware surface
					SDL_DOUBLEBUF|//double buffering
					SDL_OPENGL);//OpenGL
		for(int i=0;i<256;++i){//loop over keys
			this->KeyDown[i]=0;//key is not down
			this->KeyOffOn[i]=0;//key is off
		}
		this->MouseLeftDown=0;//left mouse button is not down
		this->MouseLeftOffOn=0;//left mouse button is off
		this->MouseRightDown=0;//right mouse button is not down
		this->MouseRightOffOn=0;//right mouse button is off
		this->MouseMiddleDown=0;//middle mouse button is not down
		this->MouseMiddleOffOn=0;//middle mouse button is off
		for(int i=0;i<2;++i){
			this->MousePosition[i]=this->WindowSize[i]/2;
			this->MouseDeltaPosition[i]=0;
		}
		this->WarpMouse=false;
		this->Running=true;
	}

	void CWindowManager::MainLoop(){
		SDL_Event E;//event
		int HandledByAntTweakBar=0;
		while(this->Running){//endless loop
			while(SDL_PollEvent(&E)){//loop over events
				if(this->UseAntTweakBar)
					HandledByAntTweakBar=TwEventSDL(&E,
							SDL_MAJOR_VERSION,SDL_MINOR_VERSION);
				if(!HandledByAntTweakBar){//is event handled by AntTweakBar
					switch(E.type){//switch over event types
						case SDL_QUIT://quit main loop
							this->Running=false;//stop running
							break;//break quit case
						case SDL_KEYDOWN://key down
							if(!this->MapKeyDown.count(E.key.keysym.sym))
								this->MapKeyDown.insert(std::pair<SDLKey,int>(E.key.keysym.sym,0));
							if(!this->MapKeyOffOn.count(E.key.keysym.sym))
								this->MapKeyOffOn.insert(std::pair<SDLKey,int>(E.key.keysym.sym,0));
							this->MapKeyDown[E.key.keysym.sym]=1;
							this->MapKeyOffOn[E.key.keysym.sym]^=1;
							if(E.key.keysym.sym==SDLK_1)exit(0);

							this->KeyDown[E.key.keysym.sym%256]=1;//key is down
							this->KeyOffOn[E.key.keysym.sym%256]^=1;//switch key state
							break;//break key down case
						case SDL_KEYUP://key up
							if(!this->MapKeyDown.count(E.key.keysym.sym))
								this->MapKeyDown.insert(std::pair<SDLKey,int>(E.key.keysym.sym,0));
							if(!this->MapKeyOffOn.count(E.key.keysym.sym))
								this->MapKeyOffOn.insert(std::pair<SDLKey,int>(E.key.keysym.sym,0));
							this->MapKeyDown[E.key.keysym.sym]=0;
							this->KeyDown[E.key.keysym.sym%256]=0;//key is not down
							break;//break key up case
						case SDL_MOUSEMOTION://mouse motion
							this->CurrentMouseTime=SDL_GetTicks();//current mouse time
							this->MousePosition[0]=E.motion.x;//x position of mouse
							this->MousePosition[1]=E.motion.y;//y position of mouse
							this->MouseDeltaPosition[0]=//delta x position
								this->MousePosition[0]-this->MouseLastPosition[0];//curr.x-prev.x
							this->MouseDeltaPosition[1]=//delta y position
								this->MousePosition[1]-this->MouseLastPosition[1];//curr.y-prev.y
							if(this->Mouse)this->Mouse();//call mouse function
							this->LastMouseTime=this->CurrentMouseTime;//current is now last
							if(	this->MousePosition[0]-this->WindowSize[0]/2!=0||//change x
									this->MousePosition[1]-this->WindowSize[1]/2!=0){//change y
								if(this->WarpMouse)//can we warp mouse position
									SDL_WarpMouse(this->WindowSize[0]/2,this->WindowSize[1]/2);//warp
							}
							this->MouseLastPosition[0]=this->MousePosition[0];//copy x position
							this->MouseLastPosition[1]=this->MousePosition[1];//copy y position
							break;//break mouse motion case
						case SDL_MOUSEBUTTONDOWN://mouse button down
							if(E.motion.state&SDL_BUTTON_LMASK){
								this->MouseLeftDown=1;//left mouse button is down
								this->MouseLeftOffOn^=1;//switch left mouse button state
							}
							if(E.motion.state&SDL_BUTTON_RMASK){
								this->MouseRightDown=1;//right mouse button is down
								this->MouseRightOffOn^=1;//switch right mouse button state
							}
							if(E.motion.state&SDL_BUTTON_MMASK){
								this->MouseMiddleDown=1;//middle mouse button is down
								this->MouseMiddleOffOn^=1;//switch middle mouse button state
							}
							break;//break mouse button down case
						case SDL_MOUSEBUTTONUP:
							if(E.motion.state&SDL_BUTTON_LMASK)
								this->MouseLeftDown=0;//left mouse button is not down
							if(E.motion.state&SDL_BUTTON_RMASK)
								this->MouseRightDown=0;//right mouse button is not down
							if(E.motion.state&SDL_BUTTON_MMASK)
								this->MouseMiddleDown=0;//middle mouse button is not down
							break;//break mouse button up case
						default://other events
							break;//break other events case
					}
				}
			}
			this->CurrentIdleTime=SDL_GetTicks();//get current idle time
			if(this->Idle)//is there idle function?
				this->Idle();//call idel function
			this->LastIdleTime=this->CurrentIdleTime;//current is now last
		}

		if(this->UseAntTweakBar)
			TwTerminate();

		SDL_Quit();
	}

	void CWindowManager::StopMainLoop(){
		this->Running=false;
	}

	void CWindowManager::Swap(){
		if(this->UseAntTweakBar)
			TwDraw();//draw tweak bar
		SDL_GL_SwapBuffers();
	}

	void CWindowManager::SetIdle(void(*Idle)()){
		this->Idle=Idle;
	}

	void CWindowManager::SetMouse(void(*Mouse)()){
		this->Mouse=Mouse;
	}

	float CWindowManager::GetDeltaIdleTime(){
		return(this->CurrentIdleTime-this->LastIdleTime)*1e-3;
	}

	float CWindowManager::GetDeltaMouseTime(){
		return(this->CurrentMouseTime-this->LastMouseTime)*1e-3;
	}

	int*CWindowManager::GetMousePosition(){
		return this->MousePosition;
	}

	int*CWindowManager::GetDeltaMousePosition(){
		return this->MouseDeltaPosition;
	}

	void CWindowManager::SetMouseWarp(bool WarpMouse){
		this->WarpMouse=WarpMouse;
	}

	unsigned*CWindowManager::GetWindowSize(){
		return this->WindowSize;
	}

	int CWindowManager::IsKeyDown(char Key){
		return this->IsKeyDown((int)Key);
	}

	int CWindowManager::IsKeyOn(char Key){
		return this->IsKeyOn((int)Key);
	}

	int CWindowManager::IsKeyDown(int Key){
		if(!this->MapKeyDown.count((SDLKey)Key))
			this->MapKeyDown.insert(std::pair<SDLKey,int>((SDLKey)Key,0));
		return this->MapKeyDown[(SDLKey)Key];
	}

	int CWindowManager::IsKeyOn(int Key){
		if(!this->MapKeyOffOn.count((SDLKey)Key))
			this->MapKeyOffOn.insert(std::pair<SDLKey,int>((SDLKey)Key,0));
		return this->MapKeyOffOn[(SDLKey)Key];
	}

	int CWindowManager::IsLeftDown(){
		return this->MouseLeftDown;
	}

	int CWindowManager::IsLeftOn(){
		return this->MouseLeftOffOn;
	}

	int CWindowManager::IsRightDown(){
		return this->MouseRightDown;
	}

	int CWindowManager::IsRightOn(){
		return this->MouseRightOffOn;
	}

	int CWindowManager::IsMiddleDown(){
		return this->MouseMiddleDown;
	}

	int CWindowManager::IsMiddleOn(){
		return this->MouseMiddleOffOn;
	}

}
