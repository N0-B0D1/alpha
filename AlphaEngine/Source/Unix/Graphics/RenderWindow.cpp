/**
Copyright 2014 Jason R. Wendlandt

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

#include "Graphics/RenderWindow.h"

namespace alpha
{
    RenderWindow::RenderWindow() { }
    RenderWindow::~RenderWindow() { }

    bool RenderWindow::Initialize()
    {
        m_pDisplay = XOpenDisplay(NULL);
 
        if(m_pDisplay == NULL) 
        {
            return false;
        }
                
        m_root = DefaultRootWindow(m_pDisplay);

        m_pVisualInfo = glXChooseVisual(m_pDisplay, 0, m_att);

        if(m_pVisualInfo == NULL) 
        {
            return false;
        }


        m_colorMap = XCreateColormap(m_pDisplay, m_root, m_pVisualInfo->visual, AllocNone);

        m_setWindowAttrs.colormap = m_colorMap;
        m_setWindowAttrs.event_mask = ExposureMask | KeyPressMask;
         
        m_window = XCreateWindow(m_pDisplay, m_root, 0, 0, 600, 600, 0, m_pVisualInfo->depth, InputOutput, m_pVisualInfo->visual, CWColormap | CWEventMask, &m_setWindowAttrs);

        XMapWindow(m_pDisplay, m_window);
        XStoreName(m_pDisplay, m_window, "ALPHA Engine");
         
        m_glContext = glXCreateContext(m_pDisplay, m_pVisualInfo, NULL, GL_TRUE);
        glXMakeCurrent(m_pDisplay, m_window, m_glContext);
         
        glEnable(GL_DEPTH_TEST); 

        return true;
    }

    bool RenderWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        XNextEvent(m_pDisplay, &m_xEvent);
        
        if(m_xEvent.type == Expose)
        {
            XGetWindowAttributes(m_pDisplay, m_window, &m_xWindowAttrs);
            glViewport(0, 0, m_xWindowAttrs.width, m_xWindowAttrs.height);
            glXSwapBuffers(m_pDisplay, m_window);
        }
                
        else if(m_xEvent.type == KeyPress)
        {            
            return false;
        }

        return true;
    }

    bool RenderWindow::Shutdown()
    {
        glXMakeCurrent(m_pDisplay, None, NULL);
        glXDestroyContext(m_pDisplay, m_glContext);
        XDestroyWindow(m_pDisplay, m_window);
        XCloseDisplay(m_pDisplay);
        return true;
    }

    void RenderWindow::Render()
    {
        this->DrawAQuad();
    }

    void RenderWindow::DrawAQuad() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1., 1., -1., 1., 1., 20.);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

        glBegin(GL_QUADS);
        glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
        glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
        glEnd();
    } 
}
