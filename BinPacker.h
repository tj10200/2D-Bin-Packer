#ifndef BIN_PACKER_H
#define BIN_PACKER_H

#include "commonHeaders.h"
#include "ImagePacker.h"
#include "Directory.h"



typedef	list<string>						Files;
typedef Files::iterator						FilesIter;
typedef map<string, Files>					DirectoryFileMap;
typedef DirectoryFileMap::iterator			DirectoryFileMapIter;
typedef Files::iterator						DirFileMapFilesIter;
typedef	list<boost::shared_ptr<ImagePacker> > Node;
typedef	Node::iterator						NodeIter;
typedef map<string, Node>					DirectoryNodes;
typedef DirectoryNodes::iterator			DirectoryNodesIter;

class BinPacker: public Directory
{
protected:
	DirectoryFileMap		m_dirFileMap;							//Holds a map of all images in each parent directory
	DirectoryNodes			m_nodes;						//Holds a map of all bins populated from each directory
	
	unsigned int			m_binNum;						//Current Bin number
	unsigned int			m_padding;						//The padding to use in between packed images
	string					m_binDir;						//Bin directory
	string					m_logFile;						//Log file location
	string					m_imgDir;						//Root Directory containing images
	
	NodeIter				addNode		(DirectoryFileMapIter i,
										 BIN_SIZE	b);		//Adds a node onto the tree
	bool					removeNode	(DirectoryNodesIter i);//Pops a node off of the tree
	DirectoryNodesIter		mergeNodes	(DirectoryNodesIter one,	//Combines two nodes into the parent node
										 DirectoryNodesIter two);

	void					mapFileDirs	();					//Populate the directory file map
	string					getBinPath	();					//Returns the full filepath of the current bin
	void					updateBinPath(int binNum);		//Sets the current bin to the new number	
	Devil				*	createBin	(unsigned int size);//Returns a new bin of specified size					

	Files					getFileHeights(list<string> f);

public:
							BinPacker	(	string			imgDir,
											string			binDir,
											unsigned int	padding, 
											string			logFile);
	virtual					~BinPacker	();

	virtual void			 packImages	();					//Iterates through images and pack them
};



#endif