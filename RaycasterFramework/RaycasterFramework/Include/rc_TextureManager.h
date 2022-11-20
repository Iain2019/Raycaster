#include <stdafx.h>
#include <rc_texture.h>

#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

//forawrd delaration for texture class
class Texture;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	//Our singleton accessor function call this function to retrieve an instance of the InputManager
	static TextureManager* GetInstance() { return m_instance; }
	//Our singleton creation function, call this to create an instance of our input manager, and return it
	static TextureManager* CreateInstance();
	//Destroy Instance this is what we call to delete the instance of our input manager
	static void DestroyInstance();

	//function to test if a texture exists in memory
	bool TextureExists(const char* a_pTextureName);
	//load a texture from filename
	Texture* LoadTexture(const char* a_pTextureName, u32 a_format);
	Texture* GetTexture(const char* a_filename);

	void ReleaseTexture(Texture* a_texturePointer);

private:
	//this is the static instance of our input manager class
	static  TextureManager* m_instance;
	//as small structure to reference count a texture
	//reference count indicates how many pointers are currently pointing to this texture
	typedef struct TextureRef
	{
		Texture* pTexture;
		unsigned int refCount;
	}TextureRef;
	std::map<std::string, TextureRef> m_pTextureMap;
};

#endif //!__TEXTURE_MANAGER_H__