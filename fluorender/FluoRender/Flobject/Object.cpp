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
#include <Flobject/Object.h>

using namespace FL;

Object::Object():
  Referenced(),
  _id(0)
{
	ValueSet* value_set = new ValueSet();
	_vs_stack.push(value_set);
}

Object::Object(const Object& obj, const CopyOp& copyop):
  Referenced(),
  _id(0)
{
	if (copyop.getCopyFlags() & CopyOp::SHALLOW_COPY)
		_vs_stack.push(obj._vs_stack.top());
	else
	{
		_vs_stack.push(obj._vs_stack.top()->clone(copyop));
		//also observe the values
		for (auto it = _vs_stack.top()->getValues().begin();
			it != _vs_stack.top()->getValues().end(); ++it)
		{
			it->second->addObserver(this);
		}
	}
}

Object::~Object()
{
}

//observer functions
void Object::objectDeleted(void* ptr)
{
	Referenced* refd = static_cast<Referenced*>(ptr);
	if (refd->className() == std::string("Object"))
		_vs_stack.top()->resetRefPtr(refd);

	//remove observee
	removeObservee(refd);
}

void Object::objectChanged(void* ptr, const std::string &exp)
{
	Referenced* refd = static_cast<Referenced*>(ptr);
	if (refd->className() == std::string("Value"))
	{
		_vs_stack.top()->syncValue(dynamic_cast<Value*>(refd));
	}
	else if (refd->className() == std::string("Object"))
	{
		//do something in response
		
	}
}

//define function bodies first
//add functions
#define OBJECT_ADD_VALUE_BODY \
	if (_vs_stack.top()) \
	{ \
		bool result = _vs_stack.top()->addValue(name, value); \
		if (result) \
		{ \
			Value* vs_value = _vs_stack.top()->findValue(name); \
			if (vs_value) \
				vs_value->addObserver(this); \
		} \
		return result; \
	} \
	else \
		return false

//actual add functions
bool Object::addValue(const std::string &name, Referenced* value)
{
	if (value) value->addObserver(this);
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, bool value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, char value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, unsigned char value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, short value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, unsigned short value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, long value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, unsigned long value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, long long value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, unsigned long long value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, float value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, double value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, const std::string &value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, const std::wstring &value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, const FLTYPE::Point &value)
{
	OBJECT_ADD_VALUE_BODY;
}

bool Object::addValue(const std::string &name, const FLTYPE::Vector &value)
{
	OBJECT_ADD_VALUE_BODY;
}

//define function bodies first
//set functions
#define OBJECT_SET_VALUE_BODY \
	if (getValue(name, old_value) && value != old_value) \
	{ \
		bool result = false; \
		if (_vs_stack.top()) \
		{ \
			result = _vs_stack.top()->setValue(name, value, notify); \
			if (result) \
				notifyObserversOfChange(name); \
		} \
		return result; \
	} \
	return false

//set functions
bool Object::setValue(const std::string &name, Referenced* value, bool notify)
{
	Referenced* old_value;
	if (getValue(name, &old_value) && value != old_value)
	{
		if (old_value)
			old_value->removeObserver(this);
		if (value)
			value->addObserver(this);
		bool result = false;
		if (_vs_stack.top())
		{
			result = _vs_stack.top()->setValue(name, value, notify);
			if (result)
				notifyObserversOfChange();
		}
		return result;
	}
	return false;
}

bool Object::setValue(const std::string &name, bool value, bool notify)
{
	bool old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, char value, bool notify)
{
	char old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, unsigned char value, bool notify)
{
	unsigned char old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, short value, bool notify)
{
	short old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, unsigned short value, bool notify)
{
	unsigned short old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, long value, bool notify)
{
	long old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, unsigned long value, bool notify)
{
	unsigned long old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, long long value, bool notify)
{
	long long old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, unsigned long long value, bool notify)
{
	unsigned long long old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, float value, bool notify)
{
	float old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, double value, bool notify)
{
	double old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, const std::string &value, bool notify)
{
	std::string old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, const std::wstring &value, bool notify)
{
	std::wstring old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, const FLTYPE::Point &value, bool notify)
{
	FLTYPE::Point old_value;
	OBJECT_SET_VALUE_BODY;
}

bool Object::setValue(const std::string &name, const FLTYPE::Vector &value, bool notify)
{
	FLTYPE::Vector old_value;
	OBJECT_SET_VALUE_BODY;
}

//define function bodies first
//get functions
#define OBJECT_GET_VALUE_BODY \
	if (_vs_stack.top()) \
		return _vs_stack.top()->getValue(name, value); \
	else \
		return false

//get functions
bool Object::getValue(const std::string &name, Referenced** value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, bool &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, char &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, unsigned char &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, short &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, unsigned short &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, long &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, unsigned long &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, long long &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, unsigned long long &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, float &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, double &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, std::string &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, std::wstring &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, FLTYPE::Point &value)
{
	OBJECT_GET_VALUE_BODY;
}

bool Object::getValue(const std::string &name, FLTYPE::Vector &value)
{
	OBJECT_GET_VALUE_BODY;
}

//bool Object::getValue(const std::string &name, Vec4f &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Vec2d &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Vec3d &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Vec4d &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Quat &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Planef &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Planed &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Matrixf &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Matrixd &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Color3f &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, Color3d &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, HsvColor3f &value)
//{
//	OBJECT_GET_VALUE_BODY
//}
//
//bool Object::getValue(const std::string &name, HsvColor3d &value)
//{
//	OBJECT_GET_VALUE_BODY
//}

//sync value
bool Object::syncValue(const std::string &name, Observer* obsrvr)
{
	if (_vs_stack.top())
	{
		Value* value = _vs_stack.top()->findValue(name);
		if (value)
		{
			value->addObserver(obsrvr);
			return true;
		}
	}
	return false;
}

//unsync value
bool Object::unsyncValue(const std::string &name, Observer* obsrvr)
{
	if (_vs_stack.top())
	{
		Value* value = _vs_stack.top()->findValue(name);
		if (value)
		{
			value->removeObserver(obsrvr);
			return true;
		}
	}
	return false;
}
