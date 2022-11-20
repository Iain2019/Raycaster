#include <stdafx.h>
#include "rc_ImageLoader.h"
#include "rc_pcxLoader.h"

void* ImageLoader::LoadFromFile(const char* a_filename, u32 a_imageType, u32& a_width, u32& a_height, u8& a_bpp, void*& a_paletteData)
{

	//Get a file stream to load from file
	std::fstream file;
	//Attempt to open file
	file.open(a_filename, std::ios_base::in | std::ios_base::binary);

	if (file.is_open()) //test to see if succesfull
	{
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fileSize = file.gcount(); //Gets the size of the file in bytes
		file.clear(); //ignore EOF flag that was previously set by reading whole file
		file.seekg(0, std::ios_base::beg);
		if (fileSize == 0) //If file has not bytes then don't try proccess it
		{
			file.close();
			return nullptr;
		}
		void* imageData = nullptr;
		switch (a_imageType)
		{
		case IM_BITMAP:
		{
			break;
		}
		case IM_PCX:
		{
			//if PCX format
			imageData = PCXLoader::LoadFromFile(&file, a_width, a_height, a_bpp, a_paletteData);
			if (a_bpp != 32)
			{
				imageData = PCXLoader::ConvertTo32bpp(imageData, a_paletteData, a_width, a_height, a_bpp);
			}
			break;
		}
		case IM_PPM:
		{
			break;
		}
		default:
		{
			file.close();
			return nullptr;
			break;
		}
		}
		file.close();
		return imageData;
	}
	return nullptr;
}