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
#ifndef _KERNELEXECUTOR_H_
#define _KERNELEXECUTOR_H_

#include <string>

namespace FL
{
	class VolumeData;
}

namespace FLIVR
{
	class KernelProgram;
}

class KernelExecutor
{
public:
	KernelExecutor();
	~KernelExecutor();

	void SetCode(std::string &code);
	void LoadCode(std::string &filename);
	void SetVolume(FL::VolumeData *vd);
	void SetDuplicate(bool dup);
	FL::VolumeData* GetVolume();
	FL::VolumeData* GetResult();
	void DeleteResult();
	bool GetMessage(std::string &msg);

	bool Execute();

private:
	FL::VolumeData *m_vd;
	FL::VolumeData *m_vd_r;//result
	bool m_duplicate;//whether duplicate the input volume

	std::string m_code;
	std::string m_message;

	bool ExecuteKernel(KernelProgram* kernel,
		GLuint data_id, void* result,
		size_t brick_x, size_t brick_y,
		size_t brick_z);

};

#endif//_KERNELEXECUTOR_H_
