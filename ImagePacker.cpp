#include "ImagePacker.h"

ImagePacker::ImagePacker (BIN_SIZE b, int padding, string outFile)
{
    m_padding				=   padding;
	
	Init						(b);
    
    m_outFile				=   outFile;
}

void ImagePacker::Init   (BIN_SIZE b)
{
	m_hasLeafs				=   false;
    m_imagePresent			=   false;
    m_padding				=   1;
	m_rLeaf.reset				();
	m_lLeaf.reset				();

	m_rc					=   Rect(m_padding, m_padding,b - m_padding,b - m_padding);
    
    return;
}

ImagePacker::~ImagePacker ()
{}
ImagePacker *ImagePacker::InsertImg  (ImagePacker *head, ImagePacker *node, BIN_SIZE b, Devil *img)
{
	//Check if image is already present
    if (node->m_imagePresent)
    {
        ImagePacker  *temp;
        //Attempt to push image into right leaf
        temp                =  node->InsertImg(head, node->m_rLeaf.get(), DEFAULT_BIN_SIZE, img);

        if (temp == NULL)
        {
			temp            =  node->InsertImg(node, node->m_lLeaf.get(), DEFAULT_BIN_SIZE, img);
           //temp            =  node->InsertImg(head->m_lLeaf.get(), head->m_lLeaf.get(), img);
        }

        return temp;

    }

    //Return null if current rectangle is too small
    if ((long)node->getWidth()   <   (long)img->m_img->getImageWidth() ||
        (long)node->getHeight()  <   (long)img->m_img->getImageHeight())
        return NULL;

	//Update Rect size, create children that surround current rect
    cout << "\nCreating Rectangles\n.";
    Rect        right       (
                                node->m_rc.getLeft() + img->m_img->getImageWidth() + m_padding + 1,
                                node->m_rc.getTop(),
                                b - m_padding,
                                node->m_rc.getTop() + img->m_img->getImageHeight()
                            ),
                bottom      (
                                node->m_rc.getLeft(),
                                node->m_rc.getTop() + img->m_img->getImageHeight() + m_padding + 1,
                                b - m_padding,
                                node->m_rc.getBottom()
                            );

    cout << "Creating Children\n.";
    node->CreateChildren    (bottom, right, b);

	//Update image flag and file path
    node->m_imagePresent	=   true;
    node->m_fileNm			=   img->getFile();

	//Return pointer to allocated image placement
    return  node;
}

ImagePacker *ImagePacker::CreateChildren (Rect left,Rect right, BIN_SIZE b)
{
    //Create right child
    if (this->m_lLeaf			==  NULL)
    {
        this->m_lLeaf.reset			(new ImagePacker(b,   this->m_padding, m_outFile));
    }

	//Reset boundary of child
    this->m_lLeaf->m_rc		=   left;

    if (this->m_rLeaf			==  NULL)
    {
        //Create right child
        this->m_rLeaf.reset			(new ImagePacker(b,   this->m_padding, m_outFile));
    }

	//Reset boundary of child
    this->m_rLeaf->m_rc		=   right;

    return  this;
}


void ImagePacker::DisplayNodeRect (Rect node, Devil *img)
{

    cout    << "Node Left: "        << node.getLeft()           << "\n"
            << "Node Right: "       << node.getRight()          << "\n"
            << "Node Top: "         << node.getTop()            << "\n"
            << "Node Bottom: "      << node.getBottom()         << "\n"
            << "Node Width: "       << node.getWidth()          << "\n"
            << "Node height: "      << node.getHeight()         << "\n"
            << "Image Width: "  << img->m_img->getImageWidth()  << "\n"
            << "Image Height: " << img->m_img->getImageHeight() << "\n\n";
}

ImagePacker *ImagePacker::PushImageIntoBin (ImagePacker *node, Devil *img, Devil *bin)
{
            //Merge image with the top left corner of current rect

        if(!bin->Merge(img->getImgID(), node->m_rc.getLeft(), node->m_rc.getTop()))
            return                    NULL;
        
		return node;
}

void ImagePacker::WriteToFile(ImagePacker *node, Devil *img, Devil *bin, XMLWriter *xml)
{
    //vars used to write data to XML file
    vector <string>     tags;
    vector <string>     elems;

    stringstream        ss;

    tags.push_back                  ("IMG_SRC");
    tags.push_back                  ("IMG_DEST");
    tags.push_back                  ("X_OFFSET");
    tags.push_back                  ("Y_OFFSET");
    tags.push_back                  ("WIDTH");
    tags.push_back                  ("HEIGHT");
    tags.push_back                  ("TEXTURE");

    elems.push_back                 (img->getFile());
    elems.push_back                 (bin->getFile());

    ss.str                          ("");
    ss                  <<          node->m_rc.getLeft();
    elems.push_back                 (ss.str());

    ss.str                          ("");
    ss                  <<          node->m_rc.getTop();
    elems.push_back                 (ss.str());

    ss.str                          ("");
    ss                  <<          img->m_img->getImageWidth();
    elems.push_back                 (ss.str());

    ss.str                          ("");
    ss                  <<          img->m_img->getImageHeight();
    elems.push_back                 (ss.str());

    xml->WriteGroup(tags, elems);
}

long ImagePacker::getOccupiedArea(ImagePacker *node, Devil *imgLoader)
{
	long						totArea			=	0;
	
	if (node			==		NULL)
		return					totArea;

	if (node->m_imagePresent)
	{
		imgLoader->Load	(node->m_fileNm);
		totArea			+=	imgLoader->m_img->getImageArea();
	}
	
	return totArea + getOccupiedArea(node->m_rLeaf.get(), imgLoader) + getOccupiedArea(node->m_lLeaf.get(), imgLoader);
}

void ImagePacker::writeNodesToFile (ImagePacker	*node, Devil *bin, XMLWriter *xml)
{
	static boost::shared_ptr<Devil>	imgLoader	(new Devil());

	//Travel down right leaf
	if (node->m_rLeaf	!= NULL)
		writeNodesToFile			(node->m_rLeaf.get(), bin, xml);

	//Travel down left leaf
	if (node->m_lLeaf	!= NULL)
		writeNodesToFile			(node->m_lLeaf.get(), bin, xml);
	
	if (node->hasImage())
	{
		imgLoader->Load(node->m_fileNm);
	
		cout << "Merging image into node.\n";
		node = PushImageIntoBin			(node,	imgLoader.get(), bin);

		//Write Data out to XML File
		WriteToFile						(node,	imgLoader.get(), bin, xml);
	}
}

ImagePacker::ImagePacker (const ImagePacker &i)
{
	this->m_rc					=	i.m_rc;
	this->m_padding				=	i.m_padding;
    this->m_hasLeafs			=	i.m_hasLeafs;
    this->m_imagePresent		=	i.m_imagePresent;
    this->m_fileNm				=	i.m_fileNm;
    this->m_outFile				=	i.m_outFile;

	if (i.m_rLeaf				!=	NULL)
		this->m_rLeaf			=	i.m_rLeaf;

	if (i.m_lLeaf				!=	NULL)
		this->m_lLeaf			=	i.m_lLeaf;
}

list<string> ImagePacker::getAllFiles (ImagePacker	*node)
{
	static list<string> temp;

	temp.clear();

	if (node->m_rLeaf			!=	NULL)
		getAllFiles				(node->m_rLeaf.get());
	if (node->m_lLeaf			!=	NULL)
		getAllFiles				(node->m_lLeaf.get());

	if (node->hasImage())
		temp.push_back			(node->getFile());
	
	return temp;
}

list<pair<string, unsigned int>> ImagePacker::getAllFileData (ImagePacker	*node)
{
	static list<pair<string, unsigned int>> temp;
	
	if (node->m_rLeaf			!=	NULL)
		getAllFileData			(node->m_rLeaf.get());
	if (node->m_lLeaf			!=	NULL)
		getAllFileData			(node->m_lLeaf.get());

	if (node->hasImage())
	{
		string		 file		=	node->getFile();
		unsigned int height		=	node->hasImage() ? node->getHeight(): 0;

		temp.push_back			(pair<string, unsigned int>(file , height));
	}

	temp.sort					(compareSecond);
	temp.unique					();
	
	return temp;
}

void ImagePacker::getAllFileData (ImagePacker	*node, list<pair<string, unsigned int>> &retList)
{
	if (node->m_rLeaf			!=	NULL)
		getAllFileData			(node->m_rLeaf.get(), retList);
	if (node->m_lLeaf			!=	NULL)
		getAllFileData			(node->m_lLeaf.get(), retList);

	if (node->hasImage())
	{
		string		 file		=	node->getFile();
		unsigned int height		=	node->hasImage() ? node->getHeight(): 0;

		retList.push_back			(pair<string, unsigned int>(file , height));
	}

	return;
}

ImagePacker	*ImagePacker::mergeTree	(ImagePacker *head, ImagePacker *newNode)
{
	list <pair<string, unsigned int>>	head_data;
	
	//Get all file data located in each node
	//head_data					=	head->getAllFileData(head);
	//head_data					=	newNode->getAllFileData(newNode);

	head->getAllFileData(head, head_data);
	newNode->getAllFileData(newNode, head_data);


	head_data.sort					(compareSecond);
	head_data.unique				();

	//New node that will be created
	ImagePacker					*temp		=	new ImagePacker((BIN_SIZE)(head->getHeight() + (head->m_padding * 2)), m_padding, head->m_outFile),
								*tempHead	=	temp;
	boost::shared_ptr<Devil>	imgLoader		(new Devil());
	list<pair<string, unsigned int>>::iterator i	=	head_data.begin();
	list<pair<string, unsigned int>>::iterator iEnd	=	head_data.end();
	
	
	//Loop through all files and merge trees into temp
	for (; i != iEnd && temp != NULL; i++)
	{
		imgLoader->Load							((*i).first);
		temp								=	temp->InsertImg	(temp, temp, (BIN_SIZE)(head->getHeight() + (head->m_padding * 2)), imgLoader.get());
		
		if (temp							!=	NULL)
			temp								=	tempHead;
	}


	return temp;
}