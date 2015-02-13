#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include<stdio.h>
#include<stdlib.h>

#include<X11/X.h>
#include<X11/Xlib.h>

#include<GL/gl.h>
#include<GL/glx.h>
//#include<GL/glu.h>

namespace alpha
{
	class RenderWindow
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

        bool Initialize();
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        void Render();

        Display * GetDisplay() const;
        Window GetWindow() const;
        XVisualInfo * GetVisualInfo() const;
        XWindowAttributes GetXWindowAttrs() const;

    private:
        /** XXX Temporary function to draw something, so we have a little more than a black window */
        //void DrawAQuad();

        Display *m_pDisplay;
        Window m_root;
        GLint m_att[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        XVisualInfo *m_pVisualInfo;
        Colormap m_colorMap;
        XSetWindowAttributes m_setWindowAttrs;
        Window m_window;
        XWindowAttributes m_xWindowAttrs;
        XEvent m_xEvent;
	};
}

#endif // RENDER_WINDOW_H
