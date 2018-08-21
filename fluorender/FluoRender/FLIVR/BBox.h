/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2004 Scientific Computing and Imaging Institute,
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

#ifndef SLIVR_BBox_h
#define SLIVR_BBox_h

#include <FLIVR/Point.h>
#include <ostream>

#ifdef _WIN32
#  undef min
#  undef max
#endif

namespace FLIVR
{
	class Vector;

	class BBox
	{
	public:

		BBox():
		is_valid_(false)
		{}

		~BBox() 
		{}

		BBox(const BBox& copy) 
			: cmin_(copy.cmin_), cmax_(copy.cmax_), is_valid_(copy.is_valid_) {}

		BBox& operator=(const BBox& copy)
		{
			is_valid_ = copy.is_valid_;
			cmin_ = copy.cmin_;
			cmax_ = copy.cmax_;
			return *this;
		}

		inline bool operator==(const BBox& bbox) const
		{
			return (cmin_ == bbox.cmin_) && (cmax_ == bbox.cmax_);
		}

		inline bool operator!=(const BBox& bbox) const
		{
			return (cmin_ != bbox.cmin_) || (cmax_ != bbox.cmax_);
		}

		BBox(const Point& min, const Point& max)
			: cmin_(min), cmax_(max), is_valid_(true) {}

		inline int valid() const { return is_valid_; }

		inline void set_valid(bool v) { is_valid_ = v; }
		inline void reset() { is_valid_ = false; }

		//! Expand the bounding box to include point p
		inline void extend(const Point& p)
		{
			if(is_valid_)
			{
				cmin_=Min(p, cmin_);
				cmax_=Max(p, cmax_);
			} 
			else 
			{
				cmin_=p;
				cmax_=p;
				is_valid_ = true;
			}
		}

		//! Extend the bounding box on all sides by a margin
		//! For example to expand it by a certain epsilon to make
		//! sure that a lookup will be inside the bounding box
		inline void extend(double val)
		{
			if (is_valid_)
			{
				cmin_.x(cmin_.x()-val); 
				cmin_.y(cmin_.y()-val); 
				cmin_.z(cmin_.z()-val); 
				cmax_.x(cmax_.x()+val); 
				cmax_.y(cmax_.y()+val); 
				cmax_.z(cmax_.z()+val);     
			}
		}

		//extend by a ratio for all axes
		inline void extend(double px, double py, double pz)
		{
			if (is_valid_)
			{
				Vector diag = diagonal();
				double valx = diag.x() * px / 2.0;
				double valy = diag.y() * py / 2.0;
				double valz = diag.z() * pz / 2.0;

				cmin_.x(cmin_.x() - valx);
				cmin_.y(cmin_.y() - valy);
				cmin_.z(cmin_.z() - valz);
				cmax_.x(cmax_.x() + valx);
				cmax_.y(cmax_.y() + valy);
				cmax_.z(cmax_.z() + valz);
			}
		}

		//! Expand the bounding box to include a sphere of radius radius
		//! and centered at point p
		inline void extend(const Point& p, double radius)
		{
			Vector r(radius,radius,radius);
			if(is_valid_)
			{
				cmin_=Min(p-r, cmin_);
				cmax_=Max(p+r, cmax_);
			} 
			else 
			{
				cmin_=p-r;
				cmax_=p+r;
				is_valid_ = true;
			}
		}

		//! Expand the bounding box to include bounding box b
		inline void extend(const BBox& b)
		{
			if(b.valid())
			{
				extend(b.min());
				extend(b.max());
			}
		}

		//! Expand the bounding box to include a disk centered at cen,
		//! with normal normal, and radius r.
		void extend_disk(const Point& cen, const Vector& normal, double r);

		//extend anisotropically
		inline void extend_ani(const Vector& v)
		{
			if (is_valid_)
			{
				Vector d = diagonal();
				cmin_ -= d * v;
				cmax_ += d * v;
			}
		}

		//clamp the box to another one
		inline void clamp(const BBox &box)
		{
			if (is_valid_)
			{
				cmin_ = Max(cmin_, box.min());
				cmax_ = Min(cmax_, box.max());
				if (!(cmin_ <= cmax_))
					is_valid_ = false;
			}
		}

		//x, y, z size
		inline Point size() const
		{
			assert(is_valid_);
			return Point(cmax_ - cmin_);
		}

		inline Point center() const
		{ assert(is_valid_); Vector d = diagonal(); return cmin_ + (d * 0.5); }

		inline double longest_edge() const
		{
			assert(is_valid_);
			Vector diagonal(cmax_-cmin_);
			return Max(diagonal.x(), diagonal.y(), diagonal.z());
		}

		inline double shortest_edge() const
		{
			assert(is_valid_);
			Vector diagonal(cmax_-cmin_);
			return Min(diagonal.x(), diagonal.y(), diagonal.z());
		}

		//! Move the bounding box 
		inline void translate(const Vector &v)
		{
			cmin_ += v;
			cmax_ += v;
		}

		//! Scale the bounding box by s, centered around o
		void scale(double s, const Vector &o);

		inline Point min() const
		{ return cmin_; }

		inline Point max() const
		{ return cmax_; }

		inline Vector diagonal() const
		{ assert(is_valid_); return cmax_-cmin_; }

		inline bool inside(const Point &p) const 
		{
			return (is_valid_ && p.x() >= cmin_.x() && 
				p.y() >= cmin_.y() && p.z() >= cmin_.z() && 
				p.x() <= cmax_.x() && p.y() <= cmax_.y() && 
				p.z() <= cmax_.z());
		}

		//! bbox's that share a face overlap
		bool overlaps(const BBox& bb) const;
		//! bbox's that share a face do not overlap_inside
		bool overlaps_inside(const BBox& bb) const;

		//! returns true if the ray hit the bbox and returns the hit point
		//! in hitNear
		bool intersect(const Point& e, const Vector& v, Point& hitNear);

		//if it intersects another bbox
		bool intersect(const BBox &box) const;

		//distance between two bboxes
		double distance(const BBox& bb) const;

		friend std::ostream& operator<<(std::ostream& out, const BBox& b);

	private:
		Point cmin_;
		Point cmax_;
		bool is_valid_;
	};

	//overlap region between two boxes
	inline BBox intersect(const BBox& box1, const BBox& box2)
	{
		BBox ibox;
		if (!box1.valid() || !box2.valid())
			return ibox;
		if (box1.max().x() < box2.min().x())
			return ibox;
		if (box1.min().x() > box2.max().x())
			return ibox;
		if (box1.max().y() < box2.min().y())
			return ibox;
		if (box1.min().y() > box2.max().y())
			return ibox;
		if (box1.max().z() < box2.min().z())
			return ibox;
		if (box1.min().z() > box2.max().z())
			return ibox;
		Point min = Max(box1.min(), box2.min());
		Point max = Min(box1.max(), box2.max());
		ibox = BBox(min, max);
		return ibox;
	}

	inline	bool BBox::intersect(const BBox &box) const
	{
		if (!valid() || !box.valid())
			return false;
		if (max().x() < box.min().x())
			return false;
		if (min().x() > box.max().x())
			return false;
		if (max().y() < box.min().y())
			return false;
		if (min().y() > box.max().y())
			return false;
		if (max().z() < box.min().z())
			return false;
		if (min().z() > box.max().z())
			return false;
		return true;
	}


} // End namespace FLIVR

#endif
