#ifndef _CWINDOWMANAGER_HPP_
#define _CWINDOWMANAGER_HPP_

#include<map>
#include<SDL/SDL.h>

namespace NDormon{

	class CWindowManager
	{
		private:
			unsigned WindowSize[2];//size of window
			bool IsFullScreen;//true if fullscreen is on
			int KeyDown[256];//1 if key is down
			int KeyOffOn[256];//1 if key was pressed odd times
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
			std::map<SDLKey,int>MapKeyDown;
			std::map<SDLKey,int>MapKeyOffOn;
		public:
			CWindowManager(unsigned Width,unsigned Height,bool FullScreen,
					void(*Idle)(),void(*Mouse)(),bool UseAntTweakBar);
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
				if(!this->MapKeyOffOn.count((SDLKey)k))
					this->MapKeyOffOn.insert(std::pair<SDLKey,int>((SDLKey)k,0));
				this->MapKeyOffOn[(SDLKey)k]=1;
			}

	};

}

#endif//_CWINDOWMANAGER_HPP_
