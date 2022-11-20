#include <stdafx.h>
#include "rc_TextureManager.h"
#include "rc_Texture.h"

TextureManager* TextureManager::m_instance = nullptr;

//create and instance of the texture manager
TextureManager* TextureManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TextureManager();
	}
	return m_instance;
}

//destory instance of the textuermanager
void TextureManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

TextureManager::TextureManager() : m_pTextureMap()
{
}

TextureManager::~TextureManager()
{
	//need to iterate through the texturemap and remove any references to images and unload
	m_pTextureMap.clear();
}
//Load Tetxture
//Uses and std map as a texture dictionary to track loaded textures
//attempts to load, a texture already in memory will incriment a ref count for that texture
Texture* TextureManager::LoadTexture(const char* a_filename, u32 a_format)
{
	Texture* pTexture = nullptr;
	if (a_filename != nullptr)
	{
		std::map<std::string, TextureRef>::iterator dictionaryIter =
			m_pTextureMap.find(a_filename);
		if (dictionaryIter != m_pTextureMap.end())
			//if iterator is not at the end then the texture was loaded
		{
			TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
			//get reference to texture ref struct
			pTexture = texRef.pTexture;
			++texRef.refCount;
		}
		else
		{
			//better load the texture in as it's not in the texture map yet
			pTexture = new Texture();
			if (pTexture->Load(a_filename, a_format))
			{
				//load successful place into texRef structure
				//and insert in dictionary
				TextureRef texRef = { pTexture, 1 };
				m_pTextureMap[a_filename] = texRef;
			}
			else { delete pTexture; pTexture = nullptr; }
			//Load failed free memory and reset to nullptr
		}
	}
	return pTexture;
}

//remover a texture from memory
//this will either unload the texture or simply decrement
//it's reference count until this reaches 0 and then unload
void TextureManager::ReleaseTexture(Texture* a_pTexture)
{
	for (auto dictionaryIter = m_pTextureMap.begin();
		dictionaryIter != m_pTextureMap.end(); ++dictionaryIter)
	{
		TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
		if (a_pTexture == texRef.pTexture) //found the texture to remove
		{
			//pre decrement will be evaluated before call to <=
			//-- it's safe code could be seperated into two lines
			if (--texRef.refCount == 0)
			{
				delete texRef.pTexture;
				texRef.pTexture = nullptr;
				m_pTextureMap.erase(dictionaryIter);
				break; //escape the for loop to return
			}
		}
	}
}
bool TextureManager::TextureExists(const char* a_filename)
{
	auto dictionaryIter = m_pTextureMap.find(a_filename);
	if (dictionaryIter != m_pTextureMap.end())
		//if iterator is not at end then texture was located
	{
		return true;
	}
	return false;
}

//return a pointer to the texture and incrementes the reference count
Texture* TextureManager::GetTexture(const char* a_filename)
{
	Texture* pTexture = nullptr;
	auto dictionaryIter = m_pTextureMap.find(a_filename);
	if (dictionaryIter != m_pTextureMap.end())
		//If iterator is not at end then texture was located
	{
		TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
		//get reference to texture ref structure
		texRef.refCount++;
		pTexture = texRef.pTexture;
	}
	return pTexture;
}