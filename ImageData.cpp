#include "ImageData.h"
void ImageData::setImageData()
{
	//Get source image data
	ILubyte     *data   =	ilGetData();

	m_bytes				=	data;
	m_width             =	ilGetInteger(IL_IMAGE_WIDTH);
	m_height            =	ilGetInteger(IL_IMAGE_HEIGHT);
	m_bytesPerPix       =	ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	if (m_bytesPerPix	==	3)
		addAlphaLayer();
	
	return;
}

void ImageData::addAlphaLayer ()
{
	ILubyte	* newData						=	(ILubyte *)malloc(m_height * m_width * 4 * sizeof(unsigned int));

	unsigned int offSet =	0;
	for (unsigned int x = 0; x < m_height * m_width * 3 * sizeof(ILubyte); x++)
	{
			//Insert transparancy layer. 
			if (x	%	3		==	0	&&
			    x				!=	0)
			{
			    newData[x + offSet]	=	0xFF;
			    offSet++;
			}
		
			newData[x + offSet]			=	m_bytes[x];	
	}

	m_bytes							=	newData;
	m_bytesPerPix					=	4;

	return;
}