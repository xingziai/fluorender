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
#ifndef _INTERFACEAGENT_H_
#define _INTERFACEAGENT_H_

#include <Flobject/Object.h>
#include <Scenegraph/Node.h>

namespace FUI
{
	class InterfaceAgent : public FL::Object
	{
	public:
		InterfaceAgent()
		{
		}

		virtual InterfaceAgent* clone(const FL::CopyOp& copyop) const { return 0; }

		virtual bool isSameKindAs(const FL::Object* obj) const
		{
			return dynamic_cast<const InterfaceAgent*>(obj) != NULL;
		}

		virtual const char* className() const { return "InterfaceAgent"; }

		//observer
		virtual void objectChanging(int notify_level, void* ptr, void* orig_node, const std::string &exp)
		{
			FL::Object::objectChanging(notify_level, ptr, orig_node, exp);
		}
		virtual void objectChanged(int notify_level, void* ptr, void* orig_node, const std::string &exp)
		{
			FL::Object::objectChanged(notify_level, ptr, orig_node, exp);
		}

		virtual void setObject(FL::Object* obj)
		{
			clearValues();
			addValue("asset", obj);
			if (obj)
			{
				copyValues(*obj);//shallow copy to share values
				UpdateAllSettings();
			}
		}
		virtual FL::Object* getObject()
		{
			FL::Object* obj = 0;
			getValue("asset", (FL::Referenced**)&obj);
			return obj;
		}

		virtual FL::Node* getObjParent()
		{
			FL::Object* obj = getObject();
			if (obj)
			{
				FL::Node* node = dynamic_cast<FL::Node*>(obj);
				if (node)
					return node->getParent(0);
			}
			return 0;
		}

		virtual void propParentValue(const std::string& name) {}

		virtual void UpdateAllSettings() {};

	protected:
	};
}

#endif//_INTERFACEAGENT_H_