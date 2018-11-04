/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2018 Scientific Computing and Imaging Institute,
University of Utah.


Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <Fui/RenderCanvasAgent.h>
#include <VRenderGLView.h>

using namespace FUI;

RenderCanvasAgent::RenderCanvasAgent(VRenderGLView &gl_view) :
	InterfaceAgent(),
	gl_view_(gl_view)
{

}

void RenderCanvasAgent::processNotification(FL::Event& event)
{
	//set values in ui
	InterfaceAgent::processNotification(event);
	if (event.getNotifyFlags() & FL::Event::NOTIFY_AGENT)
	{
		switch (event.type)
		{
		case FL::Event::EVENT_VALUE_CHANGED:
			gl_view_.m_force_clear = true;
			gl_view_.m_interactive = true;
			break;
		case FL::Event::EVENT_NODE_ADDED:
		case FL::Event::EVENT_NODE_REMOVED:
			gl_view_.PopMeshList();
			gl_view_.PopVolumeList();
			break;
		}
		gl_view_.RefreshGL(41);
	}
}

void RenderCanvasAgent::setObject(FL::RenderView* view)
{
	InterfaceAgent::setObject(view);
}

FL::RenderView* RenderCanvasAgent::getObject()
{
	return dynamic_cast<FL::RenderView*>(InterfaceAgent::getObject());
}

void RenderCanvasAgent::UpdateAllSettings()
{

}

VRenderGLView &RenderCanvasAgent::getView()
{
	return gl_view_;
}

void RenderCanvasAgent::OnBoundsChanged(FL::Event& event)
{
	gl_view_.InitView(INIT_BOUNDS | INIT_CENTER);
}