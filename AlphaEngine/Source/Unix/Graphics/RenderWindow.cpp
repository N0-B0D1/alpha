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

#include "Graphics/RenderWindow.h"

namespace alpha
{
    RenderWindow::RenderWindow() { }
    RenderWindow::~RenderWindow() { }

    bool RenderWindow::Initialize()
    {
        // spawn X display
        m_pDisplay = XOpenDisplay(NULL);
        if(m_pDisplay == NULL) 
        {
            return false;
        }
                
        m_root = DefaultRootWindow(m_pDisplay);

        // create glx visual info
        m_pVisualInfo = glXChooseVisual(m_pDisplay, 0, m_att);
        if(m_pVisualInfo == NULL) 
        {
            return false;
        }

        // set color map
        m_colorMap = XCreateColormap(m_pDisplay, m_root, m_pVisualInfo->visual, AllocNone);

        m_setWindowAttrs.colormap = m_colorMap;
        m_setWindowAttrs.event_mask = ExposureMask | KeyPressMask;

        // create window attached to display
        m_window = XCreateWindow(m_pDisplay, m_root, 0, 0, 600, 600, 0, m_pVisualInfo->depth, InputOutput, m_pVisualInfo->visual, CWColormap | CWEventMask, &m_setWindowAttrs);

        XMapWindow(m_pDisplay, m_window);
        XStoreName(m_pDisplay, m_window, "ALPHA Engine");

        return true;
    }

    bool RenderWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        XNextEvent(m_pDisplay, &m_xEvent);
        
        if(m_xEvent.type == Expose)
        {
            // on update just keep updating the window attributes
            // so that when the renderer requests them for render
            // they are up to date.
            XGetWindowAttributes(m_pDisplay, m_window, &m_xWindowAttrs);
        }
                
        else if(m_xEvent.type == KeyPress)
        {            
            return false;
        }

        return true;
    }

    bool RenderWindow::Shutdown()
    {

        XDestroyWindow(m_pDisplay, m_window);
        XCloseDisplay(m_pDisplay);
        return true;
    }

    void RenderWindow::Render()
    {
        
    }

    Display * RenderWindow::GetDisplay() const
    {
        return m_pDisplay;
    }

    Window RenderWindow::GetWindow() const
    {
        return m_window;
    }

    XVisualInfo * RenderWindow::GetVisualInfo() const
    {
        return m_pVisualInfo;
    }

    XWindowAttributes RenderWindow::GetXWindowAttrs() const
    {
        return m_xWindowAttrs;
    }
}
