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

#ifndef _SEARCHVISITOR_H_
#define _SEARCHVISITOR_H_

#include <Scenegraph/NodeVisitor.h>
#include <Scenegraph/Group.h>
#include <iostream>
#include <string>

namespace FL
{
	class SearchVisitor : public FL::NodeVisitor
	{
	public:

		SearchVisitor()
		{
			setTraversalMode(FL::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		}

		virtual void apply(FL::Node& node)
		{
			traverse(node);
		}

		virtual void apply(FL::Group& group)
		{
			traverse(group);
		}

	protected:

	};
}
#endif//_SEARCHVISITOR_H_