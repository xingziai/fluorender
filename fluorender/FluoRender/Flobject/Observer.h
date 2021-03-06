/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2014 Scientific Computing and Imaging Institute,
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
#ifndef FL_OBSERVER
#define FL_OBSERVER 1

#include <Flobject/Referenced.h>
#include <vector>
#include <string>

namespace FL
{
class Observer
{
public:
	typedef std::vector<Referenced*> Observees;

	Observer();
	virtual ~Observer();

	virtual void objectDeleted(void*) {}
	virtual void objectChanged(void*, const std::string &exp) {}

	virtual bool removeObservee(Referenced* observee);

	friend Referenced;

protected:
	Observees _observees;
};

struct ObserverInfo 
{
	Observer* observer;
	std::string expression;
};

class ObserverSet : public Referenced
{
public:

	ObserverSet(const Referenced* observedObject);

	Referenced* getObservedObject() { return _observedObject; }
	const Referenced* getObservedObject() const { return _observedObject; }

	Referenced* addRefLock();

	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void signalObjectDeleted(void* ptr);
	void signalObjectChanged(void* ptr);

	typedef std::vector<ObserverInfo> Observers;
	Observers& getObservers() { return _observers; }
	const Observers& getObservers() const { return _observers; }

	virtual const char* className() const { return "ObserverSet"; }

protected:

	ObserverSet(const ObserverSet& rhs): Referenced(rhs) {}
	ObserverSet& operator = (const ObserverSet&) {return *this; }
	virtual ~ObserverSet();

	Referenced* _observedObject;
	Observers _observers;
};

}
#endif