#ifndef IMAGE_PACKER_H
#define IMAGE_PACKER_H


#include "DevIL.h"
#include "Rectangle.h"
#include "XMLWriter.h"
struct		NodeData
{
	string	m_fileNm;
	Rect	m_rc;
};


class ImagePacker
{
private:
    Rect					m_rc;
	int                     m_padding;
    bool                    m_hasLeafs;
    bool                    m_imagePresent;
    string                  m_fileNm;
    string                  m_outFile;

    void                    DisplayNodeRect         (Rect        node,
                                                     Devil      *img);
    ImagePacker			*   PushImageIntoBin        (ImagePacker *node,
                                                     Devil      *img,
													 Devil		*bin);
	ImagePacker          *   CreateChildren          (Rect			leftChild,
                                                     Rect			rightChild,
													 BIN_SIZE		b);
    void                    WriteToFile             (ImagePacker	*node,
                                                     Devil			*img,
													 Devil			*bin,
													 XMLWriter		*xml);
public:
    boost::shared_ptr<ImagePacker> m_lLeaf;
    boost::shared_ptr<ImagePacker> m_rLeaf;
    ImagePacker                                      ()						{};
    ImagePacker                                      (BIN_SIZE	 b,
													 int         padding,
                                                     string      outFile);
    ~ImagePacker                                     ();
	void                    Init                    (BIN_SIZE		b);

    ImagePacker         *   InsertImg               (ImagePacker	*head,
                                                     ImagePacker	*node,
													 BIN_SIZE		b,
                                                     Devil			*img);
	ImagePacker			*	mergeTree				(ImagePacker	*head,
													 ImagePacker	*newNode);
	void					writeNodesToFile		(ImagePacker	*head,
													 Devil			*bin,
													 XMLWriter		*xml);
	bool                    hasLeafs                ()                          {return m_hasLeafs;}
    void                    setLeafsFlag            (bool			f)          {m_hasLeafs         = f;}
    bool                    hasImage                ()                          {return m_imagePresent;}
    void                    setHasImage             (bool			f)          {m_imagePresent     = f;}
    void                    setRect                 (Rect     		r)          {m_rc               = r;}
	long                    getWidth                ()                          {return m_rc.getWidth();}
    long                    getHeight               ()                          {return m_rc.getHeight();}
	long					getArea					()							{return getWidth() * getHeight();}
	string					getFile					()							{return m_fileNm;}
	Rect					getRect					()							{return m_rc;}
	long					getOccupiedArea			(ImagePacker	*node,
													 Devil			*imgLoader);
	list<string>			getAllFiles				(ImagePacker	*node);
	list<pair<string, unsigned int>>getAllFileData	(ImagePacker	*node);
	void					getAllFileData			(ImagePacker	*node, 
													list<pair<string, unsigned int>> &retList);

    ImagePacker          &  operator        =       (const ImagePacker &c);
	ImagePacker										(const ImagePacker &i);

	

};


#endif
