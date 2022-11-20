#include <stdafx.h>
#include "rc_pcxLoader.h"

int PCX_getEncodedByte(u8& a_value, u8& a_frequency, std::fstream* a_stream)
{
	UNREFERENCED_PARAMETER(a_value);
	UNREFERENCED_PARAMETER(a_frequency);
	UNREFERENCED_PARAMETER(a_stream);

	if (a_stream->peek() == EOF)
	{
		return EOF;
	}
	a_frequency = 1;
	a_stream->read((char*)(&a_value), 1);
	if ((a_value & PCX_RLE_MASK) == PCX_RLE_MASK)
	{
		a_frequency = a_value & PCX_RLE_FREQ_MASK;
		if (a_stream->peek() == EOF)
		{
			return EOF;
		}
		a_stream->read((char*)(&a_value), 1);
	}
	return 0;
}

void* PCXLoader::LoadFromFile(std::fstream* a_stream, u32& a_w, u32& a_h, u8& a_bpp, void*& a_palette)
{
	UNREFERENCED_PARAMETER(a_w);
	UNREFERENCED_PARAMETER(a_h);
	UNREFERENCED_PARAMETER(a_bpp);
	UNREFERENCED_PARAMETER(a_palette);

	//start filestream
	std::fstream* file = a_stream;
	PCXHeader header;
	//read 128 btyes from this file
	file->read((char*)(&header), sizeof(PCXHeader));
	//Check for valid header
	if (header.identifier != PCX_VALID_HEADER || header.encoding != PCX_RLE_ENCODING)
	{
		file->close();
		return nullptr;
	}
	//This is a valid pcx continue loading
	//Get palette info if present
	if (header.version == 3)
	{
		//ver3 had no palette and use default palette
		u8* egaPalette = (u8*)(header.colourPalette);
		for (int i = 0; i < 48; ++i)
		{
			egaPalette[i] = PCX_defaultPalette[i];
		}
	}

	if (header.bitsPerPixel == 8)
	{
		//if there are less than 8 bpp no data outside existing header palette
		file->seekg(PCX_VGA_PALLETTE_OFFSET, std::ios_base::end);
		char paletteIndicator = 0;
		file->read(&paletteIndicator, 1);
		if (paletteIndicator == 0xC) //if byte rerad in is 0xC then we have palette
		{
			a_palette = new PCXHeader::PCXPaletteColour[256];
			file->read((char*)(a_palette), 256 * sizeof(PCXHeader::PCXPaletteColour));
		}
		file->clear();
		file->seekg(0, std::ios_base::beg);
		file->seekg(sizeof(PCXHeader), std::ios_base::beg);
	}
	if (!a_palette && (header.numColourPlanes * header.bitsPerPixel) < 24)
	{
		//We haven't allocated a palette use the header palette info
		a_palette = new PCXHeader::PCXPaletteColour[16];
		memcpy(a_palette, header.colourPalette, 48);
	}
	
	//Get the oixel size of the image
	a_w = header.dimensions.right - header.dimensions.left + 1;//Width of the image in pixles
	a_h = header.dimensions.bottom - header.dimensions.top + 1;//number of scanlines in the image (pixel height)
	a_bpp = header.bitsPerPixel * header.numColourPlanes;
	//Size of the decompressed image in bytes;
	u32 bytesInRow = (u32)(a_w * (float)(a_bpp / 8.0f));
	u32 decompImageSize = a_h * bytesInRow;
	//The way we will process the image data is to decompress on image scanline at a time
	//Number of bytes in a decmpressed scanline (when colour planes greater than 1 bytes per scanline give split between R/G/B values)
	u32 decompScanLine = header.bytesPerScanline * header.numColourPlanes;
	//PCX images may contain some line padding - calculate line padding
	u32 scanlinePadding = decompScanLine - bytesInRow;
	u32 actualBytesPerImageRow = decompScanLine - scanlinePadding;
	//Create a data buffer large enough to hold the decompressed image
	u8* imageData = new u8[decompImageSize];
	memset(imageData, 0, decompImageSize);
	u8* scanlineData = new u8[decompScanLine];
	memset(scanlineData, 0, decompScanLine);

	//Create some stack variables to hold the value and frequency for the data read from file
	u8 value = 0;
	u8 frequency = 0;

	u32 bytesProccessed = 0;
	std::streamsize streamLocation;
	u32 row = 0;
	while (row < a_h - 1)
	{
		streamLocation = file->tellg();
		//for each from of the image decode the compressed image data
		for (u8* slp = scanlineData; slp < (scanlineData + decompScanLine);)
		{
			//test for premature end of file
			if (EOF == PCX_getEncodedByte(value, frequency, file))
			{
				//if file ends suddenly release and null data
				delete[] imageData;
				imageData = nullptr;
				if (!a_palette)
				{
					delete[] a_palette;
					a_palette = nullptr;
				}
				return imageData;
			}
			//for the number of runs insert the value into our image data
			for (u8 i = 0; i < frequency; ++i)
			{
				*slp++ = value;
			}
		}
		++row;
		//completing the above for loop gives us one scanline of data decompressed to copy into out image buffer

		//now copy based off number of colour planes
		if (header.numColourPlanes != 1)
		{
			//scanline is broken down into rrrrr... ggggg... bbbbb... (aaaaa)...
			//need to iterate through this imahe and copy across data to appropriate rgb channels
			u8* red = scanlineData;
			u8* green = scanlineData + header.bytesPerScanline;
			u8* blue = scanlineData + (header.bytesPerScanline * 2);
			u8* alpha = header.numColourPlanes == 4 ? scanlineData + (header.bytesPerScanline * 3) : nullptr;

			for (u32 processedBytes = bytesProccessed; processedBytes < (bytesProccessed + actualBytesPerImageRow);)
			{
				if (header.bitsPerPixel == 8)
				{
					imageData[processedBytes + 0] = *red++;
					imageData[processedBytes + 1] = *green++;
					imageData[processedBytes + 2] = *blue++;
					if (alpha != nullptr)
					{
						imageData[processedBytes + 3] = *alpha++;
					}
					processedBytes += header.numColourPlanes;
				}
				else
				{
					//format not supported yet
				}
			}
		}
		else
		{
			memcpy(&imageData[bytesProccessed], scanlineData, actualBytesPerImageRow);
		}
		bytesProccessed += actualBytesPerImageRow;
	}
	return imageData;
}

void* PCXLoader::ConvertTo32bpp(void* a_imageData, void* a_palette, u32& a_w, u32& a_h, u8& a_bpp)
{
	u8* rawImage = new u8[a_w * a_h * 4];
	u32 currentDataSize = a_w * (u32)(a_h * ((float)a_bpp / 8.0f));
	u8* currentImage = (u8*)a_imageData;
	if (a_palette != nullptr) //convert a palettised image
	{
		PCXHeader::PCXPaletteColour* palette = (PCXHeader::PCXPaletteColour*)a_palette;
		//for eachpixel in the current data set
		for (u32 pixel = 0, i = 0; pixel < currentDataSize; ++pixel, i += 4)
		{
			u32 pi = currentImage[pixel];
			if (a_bpp == 8)
			{
				rawImage[i + 0] = palette[pi].B;
				rawImage[i + 1] = palette[pi].G;
				rawImage[i + 2] = palette[pi].R;
				rawImage[i + 3] = 0;
			}
			else if (a_bpp == 4)
			{
				rawImage[i + 0] = palette[(pi >> 4) & 0xf].B;
				rawImage[i + 1] = palette[(pi >> 4) & 0xf].G;
				rawImage[i + 2] = palette[(pi >> 4) & 0xf].R;
				rawImage[i + 3] = 0;
				rawImage[i + 0] = palette[pi & 0xf].B;
				rawImage[i + 1] = palette[pi & 0xf].G;
				rawImage[i + 2] = palette[pi & 0xf].R;
				rawImage[i + 3] = 0;
			}
			else
			{
				//not supported yet
			}
		}
	}
	else //convert an RGB image to RGBA
	{
		for (u32 pixel = 0, i = 0; pixel < currentDataSize; pixel += 3, i += 4)
		{
			rawImage[i + 0] = currentImage[pixel + 2];
			rawImage[i + 1] = currentImage[pixel + 1];
			rawImage[i + 2] = currentImage[pixel + 0];
			rawImage[i + 3] = 0;
		}
	}
	delete[] a_imageData;
	a_imageData = nullptr;
	delete[] a_palette;
	a_palette = nullptr;
	return rawImage;
}