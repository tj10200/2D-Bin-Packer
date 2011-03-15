#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include "commonHeaders.h"

class ImageData
{
private:
	void				addAlphaLayer	();
	ILuint              m_width;
	ILuint              m_height;
	ILuint              m_bytesPerPix;
	ILubyte			*	m_bytes;
	
public:
	ImageData                           (){m_bytes = NULL;}
	~ImageData                          (){}
	
	ILuint				getWidth		()
	{					return m_width;}
	ILuint				getHeight		()
	{					return m_height;}
	ILuint				getbpp			()
	{					return m_bytesPerPix;}
	ILubyte			*	getBytes		()
	{					return m_bytes;}
	void                setImageData    ();                 //Updates the member vars with current image data

};

#endif