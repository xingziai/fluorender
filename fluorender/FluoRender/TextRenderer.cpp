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

#include "TextRenderer.h"
#include <FLIVR/ShaderProgram.h>
#include <FLIVR/ImgShader.h>
#include <FLIVR/VertexArray.h>
#include <FLIVR/TextureRenderer.h>

bool TextRenderer::m_init = false;
FT_Library TextRenderer::m_ft;

TextRenderer::TextRenderer(const string &lib_name)
	: m_valid(false),
	m_size(14),
	m_init_gl(false),
	m_tex(0)
{
	FT_Error err;
	if (!m_init)
	{
		err = FT_Init_FreeType(&m_ft);
		if (!err)
			m_init = true;
	}

	if (!m_init) return;

	err = FT_New_Face(m_ft, lib_name.c_str(), 0, &m_face);
	if (!err)
		m_valid = true;

	if (m_valid)
	{
		err = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE); 
		err = FT_Set_Pixel_Sizes(m_face, 0, m_size);
	}
}

TextRenderer::~TextRenderer()
{
	if (glIsTexture(m_tex))
		glDeleteTextures(1, &m_tex);
}

void TextRenderer::LoadNewFace(const string &lib_name)
{
	FT_Error err;
	if (!m_init)
	{
		err = FT_Init_FreeType(&m_ft);
		if (!err)
			m_init = true;
	}

	if (!m_init) return;

	if (m_valid)
	{
		FT_Done_Face(m_face);
		m_valid = false;
	}

	err = FT_New_Face(m_ft, lib_name.c_str(), 0, &m_face);
	if (!err)
		m_valid = true;

	if (m_valid)
	{
		err = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE); 
		err = FT_Set_Pixel_Sizes(m_face, 0, m_size);
	}
}

void TextRenderer::SetSize(unsigned int size)
{
	if (!m_valid)
		return;

	FT_Set_Pixel_Sizes(m_face, 0, size);
	m_size = size;
}

unsigned int TextRenderer::GetSize()
{
	if (!m_valid)
		return 0;
	else
		return m_size;
}

void TextRenderer::RenderText(const wstring& text, Color &color,
	float x, float y, float sx, float sy)
{
	if (!m_valid)
		return;

	GLint loc;
	if (!m_init_gl)
	{
		//texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_tex);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_init_gl = true;
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	ShaderProgram* shader =
		TextureRenderer::m_img_shader_factory.shader(IMG_SHDR_DRAW_TEXT);
	if (shader)
	{
		if (!shader->valid())
			shader->create();
		shader->bind();
		shader->setLocalParam(0, color.r(), color.g(), color.b(), 1.0f);
	}

	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	VertexArray* va_text =
		TextureRenderer::vertex_array_manager_.vertex_array(VA_Text);
	if (va_text && shader)
	{
		va_text->draw_begin();
		const wchar_t *p;
		for(p = text.c_str(); *p; p++)
		{
			if(FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
				continue;

			FT_GlyphSlot g = m_face->glyph;

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				g->bitmap.width,
				g->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				g->bitmap.buffer
				);

			float x2 = x + g->bitmap_left * sx;
			float y2 = -y - g->bitmap_top * sy;
			float w = g->bitmap.width * sx;
			float h = g->bitmap.rows * sy;

			float mat[16] = {
				w, 0.0f, 0.0f, 0.0f,
				0.0f, -h, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				x2, -y2, 0.0f, 1.0f
			};
			shader->setLocalParamMatrix(0, mat);
			va_text->draw_norm_square();

			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
		}
		va_text->draw_end();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	if (shader && shader->valid())
		shader->release();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

float TextRenderer::RenderTextLen(wstring& text)
{
	float len = 0.0f;
	if (!m_valid)
		return len;

	const wchar_t *p;
	for(p = text.c_str(); *p; p++)
	{
		if(FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = m_face->glyph;
		len += g->bitmap.width;
	}
	return len;
}