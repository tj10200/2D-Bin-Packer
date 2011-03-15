#include "BinPacker.h"


BinPacker::BinPacker (string imgDir, string	binDir, unsigned int padding, string logFile)
	: Directory	(imgDir, "*.*")
{
	m_imgDir			=	imgDir;
	m_binDir			=	binDir;
	m_logFile			=	logFile;
	m_padding			=	padding;
	m_binNum			=	0;

	mapFileDirs				();				//Initialize list of file in each directory
}
BinPacker::~BinPacker()
{}
void BinPacker::mapFileDirs ()
{
	list<string> dirs	=	Directory::getDirs(m_imgDir);				//Grabs list of directories in image dir
	list<string>			files;													//Holds list of files
	list<string>::iterator	it;
	

	for (it = dirs.begin(); it != dirs.end(); it++)						//Cycle through all directories
	{
		files			=	Directory::getFiles((*it), "*.png");		//Populate list of png's
		files.merge			(Directory::getFiles((*it), "*.jpg"));		//Merge with list of jpg's
	

		m_dirFileMap.insert	(pair<string, Files>((*it), files)); //Insert all found images into directory map
	}

	return;
}

void BinPacker::updateBinPath(int binNum = -1)
{
	if (binNum	== -1)
		binNum++;
	else
		binNum = binNum;
}
NodeIter BinPacker::addNode(DirectoryFileMapIter i, BIN_SIZE b)
{
	DirectoryNodesIter			node	=	m_nodes.find((*i).first);
	boost::shared_ptr<ImagePacker> img		(new ImagePacker	(b, m_padding, m_logFile));

	//Check if m_nodes already contains the parent directory
	if (node == m_nodes.end())									//Directory Not Found
	{
		Node								imgList;			//Declare a new Node object
		
		imgList.push_back					(img);				//Push the current bin onto the list

		m_nodes.insert						(make_pair((*i).first, imgList)); //Insert new list onto new node

		node							=	m_nodes.find((*i).first);

	}
	else//Directory found
		(*node).second.push_back			(img);				//Add bin onto list

	return --(*node).second.end();
}

bool BinPacker::removeNode(DirectoryNodesIter i)
{
	m_nodes.erase(i);
	return true;
}

string	BinPacker::getBinPath()
{
	stringstream    ss;
    ss.str                              ("");
    ss                              <<  m_binDir
                                    <<  "\\Bin"
                                    <<  m_binNum
									<<  ".png";
	return			ss.str();
}

Devil *BinPacker::createBin (unsigned int size)
{
	string			path			=	getBinPath();
	m_binNum++;

	return			new Devil			(path, size, size);
}

void BinPacker::packImages()
{
	DirectoryFileMap::iterator	di	=	m_dirFileMap.begin();
	NodeIter					ni;
	boost::shared_ptr<Devil>	img			(new Devil());
	ImagePacker				*	head	=	NULL;
	ImagePacker				*	temp	=	NULL;

	for (; di != m_dirFileMap.end(); di++)
	{
		DirFileMapFilesIter		fi		=	(*di).second.begin();
		
		//Push the parent node onto the map
		ni								=	addNode(di, DEFAULT_BIN_SIZE);

		for (; fi != (*di).second.end(); fi++)
		{
			img->Load						((*fi));
			
			temp						= 	(*ni).get();
			head						=	temp;
			
			temp						=	temp->InsertImg(temp, temp, DEFAULT_BIN_SIZE, img.get());
			temp						=	head;

		}
	}
	
	return;
}