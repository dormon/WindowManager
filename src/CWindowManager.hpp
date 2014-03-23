#ifndef _CWINDOWMANAGER_HPP_
#define _CWINDOWMANAGER_HPP_

#include<iostream>
#include<map>

#define USE_SDL2

#define NDormon_CWindowManager_WINDOWSIZE    NULL
#define NDormon_CWindowManager_WIDTH         100
#define NDormon_CWindowManager_HEIGHT        100
#define NDormon_CWindowManager_FULLSCREEN    false
#define NDormon_CWindowManager_IDLE          NULL
#define NDormon_CWindowManager_MOUSE         NULL
#define NDormon_CWindowManager_ANTTWEAKBAR   false
#define NDormon_CWindowManager_WINDOWNAME    "SDL Window"
#define NDormon_CWindowManager_OGL_VERSION   430
#define NDormon_CWindowManager_STENCIL       8
#define NDormon_CWindowManager_DEPTH         24
#define NDormon_CWindowManager_MSG_BOX_TITLE "Message Box"
#define NDormon_CWindowManager_MSG_BOX_FLAGS SDL_MESSAGEBOX_INFORMATION
#define NDormon_CWindowManager_MSG_BOX_MSG   "Message"
#define NDormon_CWindowManager_KEYS          256
#define NDormon_CWindowManager_Version       430
#define NDormon_CWindowManager_Profile       SDL_GL_CONTEXT_PROFILE_CORE
#define NDormon_CWindowManager_ContextFlag   SDL_GL_CONTEXT_DEBUG_FLAG

#ifdef USE_SDL2
	#include<SDL2/SDL.h>
#else
	#include<SDL/SDL.h>
#endif//USE_SDL2

namespace NDormon{
	class CWindowManager
	{
		private:
			unsigned WindowSize[2];//size of window
			bool IsFullScreen;//true if fullscreen is on
			int KeyDown [NDormon_CWindowManager_KEYS];//1 if key is down
			int KeyOffOn[NDormon_CWindowManager_KEYS];//1 if key was pressed odd times
      int KeyHit  [NDormon_CWindowManager_KEYS];
			int MouseLeftDown;//1 if left button of mouse is down
			int MouseLeftOffOn;//1 if left button of mouse was pressed odd times
			int MouseRightDown;//1 if right button of mouse is down
			int MouseRightOffOn;//1 if right button of mouse was pressed odd times
			int MouseMiddleDown;//1 if middle button of mouse is down
			int MouseMiddleOffOn;//1 if middle button of mouse was pressd odd times
			int MousePosition[2];//position of mouse cursor
			int MouseLastPosition[2];//prev. position of mouse
			int MouseDeltaPosition[2];//curr. position - prev. position of mouse
			bool WarpMouse;//if true set mouse position to the center of the screen
			bool Running;//if true application is running
			void(*Idle)();//idle call back function
			void(*Mouse)();//mouse call back function
			bool UseAntTweakBar;//use anttweakbar flag
			Uint32 ElapsedTime;//elapsed time
			Uint32 LastMouseTime;//last mouse time
			Uint32 LastIdleTime;//last idle time
			Uint32 CurrentMouseTime;//current mouse time
			Uint32 CurrentIdleTime;//current idle time
#ifdef USE_SDL2
			std::map<SDL_Keycode,int>MapKeyDown;
			std::map<SDL_Keycode,int>MapKeyOffOn;
      std::map<SDL_Keycode,int>MapKeyHit;
			SDL_Window*MainWindow;
			SDL_GLContext MainContext;
#else
			std::map<SDLKey,int>MapKeyDown;
			std::map<SDLKey,int>MapKeyOffOn;
#endif//USE_SDL2
      void Constructor(unsigned Width,unsigned Height,bool FullScreen,
					void(*Idle)(),void(*Mouse)(),bool UseAntTweakBar,unsigned Version,
          SDL_GLprofile Profile,SDL_GLcontextFlag ContextFlag);
		public:
			CWindowManager(
          unsigned Width=NDormon_CWindowManager_WIDTH,
          unsigned Height=NDormon_CWindowManager_HEIGHT,
          bool FullScreen=NDormon_CWindowManager_FULLSCREEN,
					void(*Idle)()=NDormon_CWindowManager_IDLE,
          void(*Mouse)()=NDormon_CWindowManager_MOUSE,
          bool UseAntTweakBar=NDormon_CWindowManager_ANTTWEAKBAR,
          unsigned Version=NDormon_CWindowManager_Version,
          SDL_GLprofile Profile=NDormon_CWindowManager_Profile,
          SDL_GLcontextFlag ContextFlag=NDormon_CWindowManager_ContextFlag
          );
			~CWindowManager();
			SDL_GLContext GetContext();
			SDL_Window*GetWindow();
			void MessageBox(
          std::string Message=NDormon_CWindowManager_MSG_BOX_MSG,
          std::string Title=NDormon_CWindowManager_MSG_BOX_TITLE,
					Uint32 flags=NDormon_CWindowManager_MSG_BOX_FLAGS);
			void ChangeTitle(const char*Title);
			void MainLoop();
			void StopMainLoop();
			void Swap();
			void SetIdle(void(*Idle)());
			void SetMouse(void(*Mouse)());
			float GetDeltaIdleTime();
			float GetDeltaMouseTime();
			int*GetMousePosition();
			int*GetDeltaMousePosition();
			void SetMouseWarp(bool WrapMouse);
			unsigned*GetWindowSize();

      int WasKeyHit(int Key);

			int IsKeyDown(int Key);
			int IsKeyOn(int Key);
			int IsKeyDown(char Key);
			int IsKeyOn(char Key);
			int IsLeftDown();
			int IsLeftOn();
			int IsRightDown();
			int IsRightOn();
			int IsMiddleDown();
			int IsMiddleOn();
			void SetKeyOn(int k){
				this->KeyOffOn[k]=1;
#ifdef USE_SDL2
				if(!this->MapKeyOffOn.count((SDL_Keycode)k))
					this->MapKeyOffOn.insert(std::pair<SDL_Keycode,int>((SDL_Keycode)k,0));
				this->MapKeyOffOn[(SDL_Keycode)k]=1;
#else
				if(!this->MapKeyOffOn.count((SDLKey)k))
					this->MapKeyOffOn.insert(std::pair<SDLKey,int>((SDLKey)k,0));
				this->MapKeyOffOn[(SDLKey)k]=1;
#endif//USE_SDL2

			}

	};

}

#endif//_CWINDOWMANAGER_HPP_
