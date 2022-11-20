#include <stdafx.h>
#pragma once
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

//A class to store texture data
//A texture is just a data buffer that contains some pixel values
//Values are specified by meber vatiables of the texture class

class Texture
{
public:
	//constructor
	Texture();
	//destructor
	~Texture();

	//Functionality to load a texture from a provided file name
	bool Load(const char* a_filename, u32 a_format);

	//Getters to retrieve texture information
	const std::string& GetFileName() const { return m_filename; }
	void GetDimensions(u32& a_Width, u32& a_Height) const;
	u8 GetBPP() { return m_bpp; }
	//get the palette information from the image
	//return value is palette count
	//parameter returns points to palette data in R8G8B8 format
	u16 GetPalette(void* a_palette);
	void* GetPixelData() { return m_pixelData; }

private:
	std::string m_filename;
	u32 m_width;
	u32 m_height;
	u16 m_paletteSize;
	u8 m_bpp;
	void* m_palette;
	void* m_pixelData;
};

#endif //!__TEXTURE_H_