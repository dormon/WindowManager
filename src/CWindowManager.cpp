#include"CWindowManager.hpp"

#include<iostream>
#include<AntTweakBar.h>

namespace NDormon{
  void CWindowManager::Constructor(
      unsigned Width,
      unsigned Height,
      bool FullScreen,
      void(*Idle)(),
      void(*Mouse)(),
      bool UseAntTweakBar,
      unsigned Version,
      SDL_GLprofile Profile,
      SDL_GLcontextFlag ContextFlag){
		this->MapKeyDown.clear();
		this->MapKeyOffOn.clear();
		this->Idle=Idle;//set idle function
		this->Mouse=Mouse;//set mouse function
		this->WindowSize[0]=Width;//width of window
		this->WindowSize[1]=Height;//height of window
		this->IsFullScreen=FullScreen;//set fullscreen flag
		this->UseAntTweakBar=UseAntTweakBar;


		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);//initialise video
#ifdef USE_SDL2
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,Version/100);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(Version%100)/10);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,Profile);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,ContextFlag);
#endif//USE_SDL2
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
#ifdef USE_SDL2
		if(this->IsFullScreen)
			this->MainWindow=SDL_CreateWindow(
					"SDL2",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					this->WindowSize[0],
					this->WindowSize[1],
					SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN);
		else
			this->MainWindow=SDL_CreateWindow(
					"SDL2",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					this->WindowSize[0],
					this->WindowSize[1],
					SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
		this->MainContext=SDL_GL_CreateContext(this->MainWindow);
#else
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
#endif//USE_SDL2

		for(int i=0;i<256;++i){//loop over keys
			this->KeyDown[i]=0;//key is not down
			this->KeyOffOn[i]=0;//key is off
		}
		if(this->UseAntTweakBar){
			TwInit(TW_OPENGL_CORE,NULL);
			TwWindowSize(this->WindowSize[0],this->WindowSize[1]);
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

  CWindowManager::CWindowManager(
      unsigned Width,
      unsigned Height,
      bool FullScreen,
      void(*Idle)(),
      void(*Mouse)(),
      bool UseAntTweakBar,
      unsigned Version,
      SDL_GLprofile Profile,
      SDL_GLcontextFlag ContextFlag){
    this->Constructor(Width,Height,FullScreen,Idle,Mouse,UseAntTweakBar,Version,Profile,ContextFlag);
  }
  /*
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


		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);//initialise video
#ifdef USE_SDL2
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);
#endif//USE_SDL2
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

#ifdef USE_SDL2
		if(this->IsFullScreen)
			this->MainWindow=SDL_CreateWindow(
					"SDL2",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					this->WindowSize[0],
					this->WindowSize[1],
					SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN);
		else
			this->MainWindow=SDL_CreateWindow(
					"SDL2",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					this->WindowSize[0],
					this->WindowSize[1],
					SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
		this->MainContext=SDL_GL_CreateContext(this->MainWindow);
#else
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
#endif//USE_SDL2
		for(int i=0;i<256;++i){//loop over keys
			this->KeyDown[i]=0;//key is not down
			this->KeyOffOn[i]=0;//key is off
		}
		if(this->UseAntTweakBar){
			TwInit(TW_OPENGL_CORE,NULL);
			TwWindowSize(this->WindowSize[0],this->WindowSize[1]);
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
*/
	CWindowManager::~CWindowManager(){
		if(this->UseAntTweakBar){
			TwTerminate();
		}
	}

	SDL_GLContext CWindowManager::GetContext(){
		return this->MainContext;
	}
	SDL_Window*CWindowManager::GetWindow(){
		return this->MainWindow;
	}
	void CWindowManager::MessageBox(
			std::string Message,
			std::string Title,
			Uint32 flags){
		SDL_ShowSimpleMessageBox(flags,Title.c_str(),Message.c_str(),this->MainWindow);
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
#ifdef USE_SDL2
						case SDL_KEYDOWN://key down
							if(!this->MapKeyDown.count(E.key.keysym.sym))
								this->MapKeyDown.insert(std::pair<SDL_Keycode,int>(E.key.keysym.sym,0));
							if(!this->MapKeyOffOn.count(E.key.keysym.sym))
								this->MapKeyOffOn.insert(std::pair<SDL_Keycode,int>(E.key.keysym.sym,0));
							this->MapKeyDown[E.key.keysym.sym]=1;
							this->MapKeyOffOn[E.key.keysym.sym]^=1;
							if(E.key.keysym.sym==SDLK_1)exit(0);
              if(E.key.keysym.sym>=NDormon_CWindowManager_KEYS)break;

							this->KeyDown[E.key.keysym.sym%256]=1;//key is down
							this->KeyOffOn[E.key.keysym.sym%256]^=1;//switch key state
							break;//break key down case
						case SDL_KEYUP://key up
							if(!this->MapKeyDown.count(E.key.keysym.sym))
								this->MapKeyDown.insert(std::pair<SDL_Keycode,int>(E.key.keysym.sym,0));
							if(!this->MapKeyOffOn.count(E.key.keysym.sym))
								this->MapKeyOffOn.insert(std::pair<SDL_Keycode,int>(E.key.keysym.sym,0));
							this->MapKeyDown[E.key.keysym.sym]=0;
							this->KeyDown[E.key.keysym.sym%256]=0;//key is not down
							break;//break key up case
#else
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
#endif
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
#ifdef USE_SDL2
									SDL_WarpMouseInWindow(this->MainWindow,this->WindowSize[0]/2,this->WindowSize[1]/2);//warp
#else
									SDL_WarpMouse(this->WindowSize[0]/2,this->WindowSize[1]/2);//warp
#endif
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
#ifdef USE_SDL2
		SDL_GL_SwapWindow(this->MainWindow);
#else
		SDL_GL_SwapBuffers();
#endif//USE_SDL2
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
#ifdef USE_SDL2
		if(!this->MapKeyDown.count((SDL_Keycode)Key))
			this->MapKeyDown.insert(std::pair<SDL_Keycode,int>((SDL_Keycode)Key,0));
		return this->MapKeyDown[(SDL_Keycode)Key];
#else
		if(!this->MapKeyDown.count((SDLKey)Key))
			this->MapKeyDown.insert(std::pair<SDLKey,int>((SDLKey)Key,0));
		return this->MapKeyDown[(SDLKey)Key];
#endif//USE_SDL2
	}

	int CWindowManager::IsKeyOn(int Key){
#ifdef USE_SDL2
		if(!this->MapKeyOffOn.count((SDL_Keycode)Key))
			this->MapKeyOffOn.insert(std::pair<SDL_Keycode,int>((SDL_Keycode)Key,0));
		return this->MapKeyOffOn[(SDL_Keycode)Key];
#else
		if(!this->MapKeyOffOn.count((SDLKey)Key))
			this->MapKeyOffOn.insert(std::pair<SDLKey,int>((SDLKey)Key,0));
		return this->MapKeyOffOn[(SDLKey)Key];
#endif//USE_SDL2
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
