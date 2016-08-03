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
#include "dbscan.h"

using namespace FL;

ClusterDbscan::ClusterDbscan():
	m_size(60),
	m_eps(2.8f),
	m_intw(0.0f),
	m_id_counter(1)
{

}

ClusterDbscan::~ClusterDbscan()
{

}

void ClusterDbscan::AddClusterPoint(const FLIVR::Point &p, const float value)
{
	pClusterPoint pp(new ClusterPoint);
	pp->id = m_id_counter++;
	pp->visited = false;
	pp->center = p;
	pp->intensity = value;
	m_data.push_back(pp);
}

bool ClusterDbscan::Execute()
{
	for (ClusterIter iter = m_data.begin();
		iter != m_data.end(); ++iter)
	{
		pClusterPoint p = *iter;
		if (p->visited)
			continue;
		p->visited = true;
		Cluster neighbors = GetNeighbors(p);
		if (neighbors.size() >= m_size)
		{
			Cluster cluster;
			ExpandCluster(p, neighbors, cluster);
			m_result.push_back(cluster);
		}
	}
	
	//remove noise

	return true;
}

void ClusterDbscan::ExpandCluster(pClusterPoint& p, Cluster& neighbors, Cluster& cluster)
{
	cluster.push_back(p);
	for (ClusterIter iter = neighbors.begin();
		iter != neighbors.end(); ++iter)
	{
		pClusterPoint p2 = *iter;
		if (!p2->visited)
		{
			p2->visited = true;
			Cluster neighbors2 = GetNeighbors(p2);
			if (neighbors2.size() >= m_size)
				neighbors.join(neighbors2);
		}
		if (!m_result.find(p2))
			cluster.push_back(p2);
	}
}

Cluster ClusterDbscan::GetNeighbors(pClusterPoint &p)
{
	Cluster neighbors;

	for (ClusterIter iter = m_data.begin();
		iter != m_data.end(); ++iter)
	{
		if (Dist(*p, **iter, m_intw) < m_eps)
			neighbors.push_back(*iter);
	}

	return neighbors;
}

void ClusterDbscan::GenerateNewIDs(unsigned int id, void* label,
	size_t nx, size_t ny, size_t nz)
{
	unsigned int id2 = id;
	unsigned long long index;
	int i, j, k;

	for (size_t ii = 0; ii < m_result.size(); ++ii)
	{
		Cluster &cluster = m_result[ii];
		do
		{
			id2 += 20;
			if (id2 == id)
				break;
		}
		while (!id2 || FindId(label, id2, nx, ny, nz));

		for (ClusterIter iter = cluster.begin();
			iter != cluster.end(); ++iter)
		{
			i = int((*iter)->center.x() + 0.5);
			if (i <= 0 || i >= nx - 1)
				continue;
			j = int((*iter)->center.y() + 0.5);
			if (j <= 0 || j >= ny - 1)
				continue;
			k = int((*iter)->center.z() + 0.5);
			if (k <= 0 || k >= nz - 1)
				continue;
			index = nx*ny*k + nx*j + i;
			((unsigned int*)label)[index] = id2;
		}
	}
}

bool ClusterDbscan::FindId(void* label, unsigned int id,
	size_t nx, size_t ny, size_t nz)
{
	unsigned long long for_size = (unsigned long long)nx *
		(unsigned long long)ny * (unsigned long long)nz;
	unsigned long long index;
	for (index = 0; index < for_size; ++index)
	{
		if (((unsigned int*)label)[index] == id)
			return true;
	}
	return false;
}