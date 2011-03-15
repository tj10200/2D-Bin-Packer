#ifndef IMAGE_H
#define IMAGE_H

#include "commonHeaders.h"
#include "ImageData.h"

class Image
{
private:
	boost::shared_ptr<ImageData> m_imgData;
	string						m_filePath;

public:
	Image						(string filePath)
	{
		m_imgData.reset			(new ImageData());
		
		m_filePath		=		filePath;
	}
	~Image						()						{}

	ILuint						getImageWidth()			{return		m_imgData->getWidth();}
	ILuint						getImageHeight()		{return		m_imgData->getHeight();}
	ILuint						getImageArea()			{return		m_imgData->getWidth() * m_imgData->getHeight();}
	ILuint						getBPP		()			{return		m_imgData->getbpp();}
	ILubyte				*		getBytes	()			{return		m_imgData->getBytes();}
	string						getFilePath ()			{return		m_filePath;}

	void						repopImageData()		{			m_imgData->setImageData();}

};

#endif