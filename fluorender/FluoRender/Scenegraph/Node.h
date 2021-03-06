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

#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <Flobject/CopyOp.h>
#include <Flobject/Object.h>
#if defined(__linux__)
#include <algorithm>
#endif

namespace FL
{
	class Group;
	class Node;
	typedef std::vector<Node*> ParentList;
	typedef std::vector<ref_ptr<Node>> NodeList;

	class Node : public Object
	{
	public:
		Node();
		Node(const Node&, const CopyOp& copyop = CopyOp::SHALLOW_COPY);

		virtual Object* clone(const CopyOp& copyop) const
		{ return new Node(*this, copyop); }

		virtual bool isSameKindAs(const Object* obj) const
		{ return dynamic_cast<const Node*>(obj) != NULL; }

		virtual const char* className() const { return "Node"; }

		/* parents
		*/
		inline const ParentList& getParents() const { return m_parents; }

		inline ParentList getParents() { return m_parents; }

		inline const Node* getParent(unsigned int i) const { return m_parents[i]; }

		inline Node* getParent(unsigned int i) { return m_parents[i]; }

		inline unsigned int getNumParents() const { return static_cast<unsigned int>(m_parents.size()); }

	protected:
		virtual ~Node();
		void addParent(Node* node);
		void removeParent(Node* node);

		ParentList m_parents;
		friend class Group;
	};
}

#endif//_NODE_H_
