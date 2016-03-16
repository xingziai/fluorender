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
#include <wx/wx.h>

#ifndef _CALCULATIONDLG_H_
#define _CALCULATIONDLG_H_

class VRenderView;
class VolumeData;

class CalculationDlg : public wxPanel
{
public:
	enum
	{
		//calculations
		//operand A
		ID_CalcLoadABtn = wxID_HIGHEST + 2501,
		ID_CalcAText,
		//operand B
		ID_CalcLoadBBtn,
		ID_CalcBText,
		//two-opeartors
		ID_CalcSubBtn,
		ID_CalcAddBtn,
		ID_CalcDivBtn,
		ID_CalcIscBtn,
		//one-opeartors
		ID_CalcFillBtn
	};

	CalculationDlg(wxWindow* frame,
		wxWindow* parent);
	~CalculationDlg();

private:
	wxWindow* m_frame;

	//current view
	VRenderView *m_cur_view;
	//current volume
	VolumeData *m_vol1;
	VolumeData *m_vol2;

	//calculations
	//operands
	wxButton *m_calc_load_a_btn;
	wxTextCtrl *m_calc_a_text;
	wxButton *m_calc_load_b_btn;
	wxTextCtrl *m_calc_b_text;
	//two-operators
	wxButton *m_calc_sub_btn;
	wxButton *m_calc_add_btn;
	wxButton *m_calc_div_btn;
	wxButton *m_calc_isc_btn;
	//one-operators
	wxButton *m_calc_fill_btn;

private:
	//calculations
	//operands
	void OnLoadA(wxCommandEvent &event);
	void OnLoadB(wxCommandEvent &event);
	//operators
	void OnCalcSub(wxCommandEvent &event);
	void OnCalcAdd(wxCommandEvent &event);
	void OnCalcDiv(wxCommandEvent &event);
	void OnCalcIsc(wxCommandEvent &event);
	//one-operators
	void OnCalcFill(wxCommandEvent &event);

	DECLARE_EVENT_TABLE();
};

#endif//_CALCULATIONDLG_H_