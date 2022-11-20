#include <stdafx.h>
#include "rc_texture.h"
#include "rc_ImageLoader.h"

Texture::Texture() :
	m_filename(""), m_width(0), m_height(0),
	m_bpp(0), m_paletteSize(0), m_palette(nullptr), m_pixelData(nullptr)
{
}

Texture::~Texture()
{
	//If these are not null then image was loaded, call delete[] on them
	if (m_palette) { delete[] m_palette; m_palette = nullptr; }
	if (m_pixelData) { delete[] m_pixelData; m_pixelData = nullptr; }
}

bool Texture::Load(const char* a_filename, u32 a_format)
{
	m_pixelData = ImageLoader::LoadFromFile(a_filename, a_format, m_width,
		m_height, m_bpp, m_palette);
	if (m_pixelData)
	{
		m_filename = a_filename;
		return true;
	}
	m_width = m_height = 0;
	m_bpp = 0;
	m_paletteSize = 0;
	return false;
}

void Texture::GetDimensions(u32& a_Width, u32& a_Height) const
{
	a_Width = m_width;
	a_Height = m_height;
}

u16 Texture::GetPalette(void* a_palette)
{
	a_palette = m_palette;
	return m_paletteSize;
}