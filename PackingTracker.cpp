#include "PackingTracker.h"

//This function is used as in the file map heights list sorting
bool compareSecond(pair<string, unsigned int> first, pair<string, unsigned int> second)
{
	return first.second > second.second;
}


PackingTracker::PackingTracker	(string	 imgDir, string	binDir, unsigned int padding, string logFile)
	: BinPacker (imgDir, binDir, padding, logFile)
{

	initMaps					();
	sortDirFileMap				();

}

void PackingTracker::initMaps()
{
	DirectoryFileMapIter	i		=	m_dirFileMap.begin();
	FilesIter				fi;		
	shared_ptr<Devil>		img			(new Devil());

	//Loop through entire file list
	for (; i != m_dirFileMap.end(); i++)
	{
		m_completeDirs.insert			(make_pair((*i).first,false));				//Initialize completed directories

		fi							=	(*i).second.begin();						//Loop through all files
		
		FileHeights			l;														//Create list to push into map
			
		for (; fi != (*i).second.end(); fi++)
		{
			img->Load					((*fi));									//Load image
			
			l.push_back					(make_pair((*fi), img->m_img->getImageHeight()));
								
		}

		m_fileMapHeights.insert		(make_pair((*i).first, l));						//Insert image list into parent directory
	}

}

void PackingTracker::sortDirFileMap()
{
	sortFileHeights				();
	reinitDirFileMap			();

}
void PackingTracker::sortFileHeights()
{
	FileMapHeightsIter i	=	m_fileMapHeights.begin();

	for (; i != m_fileMapHeights.end(); i++)
	{
		(*i).second.sort		(compareSecond);
		(*i).second.unique		();
	}

}

Files PackingTracker::sortFileList(Files f, Devil *imgLoader)
{
	FileHeights heightList;

	for (FilesIter i = f.begin(); i != f.end(); i++)
	{
		imgLoader->Load			((*i));
		heightList.push_back	(make_pair((*i), imgLoader->m_img->getImageHeight()));
	}
		
	heightList.sort				(compareSecond);
	heightList.unique			();
	
	Files		retList;

	for (FileHeightsIter i = heightList.begin(); i != heightList.end(); i++)
		retList.push_back		((*i).first);

	return retList;
}

void PackingTracker::reinitDirFileMap ()
{
	DirectoryFileMapIter di	=	BinPacker::m_dirFileMap.begin();
	FileMapHeightsIter	fi	=	m_fileMapHeights.begin();

	for (; di != BinPacker::m_dirFileMap.end(); di++)									//Clear out the old data
		(*di).second.clear();
	m_dirFileMap.clear();

	for (; fi != m_fileMapHeights.end(); fi++)											//Insert the new file lists
	{
		FileHeightsIter fh	=	(*fi).second.begin();		
		Files			f;

		for (; fh != (*fi).second.end(); fh++)											//Iterate through file heights list and 
			f.push_back			((*fh).first);											// push into orig files list

		m_dirFileMap.insert		(make_pair((*fi).first, f));							//Insert into orig file map
	}

}

void PackingTracker::packImages()
{
	DirectoryFileMapIter		di		=	BinPacker::m_dirFileMap.begin();
	
	for (; di != BinPacker::m_dirFileMap.end(); di++)
	{
		//Attempt push at default bin size
		if (!(m_completeDirs[(*di).first]=	packDirectory(di, DEFAULT_BIN_SIZE)))		//Set flag if directory successfully packed
		{
			DirectoryNodesIter	i		=	m_nodes.find((*di).first);
	
			if ((m_completeDirs[(*di).first]	= packDirectory(di, LARGE_BIN_SIZE)))	//Try node again at larger size
			{
				if (m_failedFiles.find((*di).first) != m_failedFiles.end())				//Remove failed files from list if succeded
					m_failedFiles.erase((*di).first);
	
				(*i).second.pop_front		();											//Remove first node
			}
			else
				(*i).second.pop_back		();											//Remove newly created node
		}
	}

	cleanNodes								();											//Delete any zero sized nodes

	DirectoryNodes							n;

	n								=		mergeDirectories (DEFAULT_BIN_SIZE);
			
	writeBins								(n);										//Write bins to file
}

bool PackingTracker::packDirectory	(DirectoryFileMapIter i, BIN_SIZE b)
{
	NodeIter					ni;														//Holds the directory tree structure
	boost::shared_ptr<Devil>	img			(new Devil());								//Used for loading images
	ImagePacker				*	head	=	NULL;										//holds the head node
	ImagePacker				*	temp	=	NULL;										//Holds the nodes used in tree operations
	bool						packed	=	true;										//Is entire directory packed

	DirFileMapFilesIter			fi		=	(*i).second.begin();						//Set the file iterator
	Files						file;													//List of failed files	
	
	ni								=	BinPacker::addNode(i, b);						//Push the parent node onto the map
	
	for (; fi != (*i).second.end(); fi++)												//Loop through all available files
	{
		img->Load						((*fi));
		
		if	(img->m_img->getImageArea()	> MAX_PACKABLE_AREA)							//Do not pack images that are too large
			temp					=	NULL;
		else
		{
			temp					= 	(*ni).get();									//Assign node location to temp
			head					=	temp;											//Track node start location with head
			
			temp					=	temp->InsertImg(temp, temp, DEFAULT_BIN_SIZE, img.get()); //Find spot for image
		}

		if (temp					==	NULL)											//Log failed files on failure
		{
			packed					=	false;
			file.push_back				((*fi));										//Insert image into list
			m_failedFiles.insert		(make_pair((*i).first, file));					//push into failed files map
		}	

		temp						=	head;											//Reassign temp to head
	}


	return packed;																		//Return entire directory packed
}

void PackingTracker::cleanNodes	()														
{
	DirectoryNodesIter	di				=	m_nodes.begin();
	NodeIter			ni;
	boost::shared_ptr<Devil> imgLoader		(new Devil());

	for(; di != m_nodes.end(); di++)
	{
		ni							=	(*di).second.begin();
	
		while (ni != (*di).second.end())
		{
			if ((*ni)->getOccupiedArea	((*ni).get(), imgLoader.get())	==	0)
			{
				(*di).second.remove		((*ni));
				ni					=	(*di).second.begin();
			}
			else
				ni++;
		}
	}
}
void PackingTracker::writeBins (DirectoryNodes n)
{
	DirectoryNodesIter			dni;	
	NodeIter					ni;
	boost::shared_ptr<Devil>	bin;
	boost::shared_ptr<XMLWriter> xml	(new XMLWriter(BinPacker::m_logFile));

	dni								=	n.begin();
	for (; dni != n.end(); dni++)													//Loop through all nodes
	{
		ni							=	(*dni).second.begin();
		
		for (; ni != (*dni).second.end(); ni++)											//Write all nodes to file
		{
			bin.reset					(new Devil(getBinPath(), (*ni)->getHeight(), (*ni)->getHeight()));
			BinPacker::updateBinPath	(++BinPacker::m_binNum);
			(*ni)->writeNodesToFile		((*ni).get(), bin.get(), xml.get());
		}
	}

	writeFailedFiles					(xml.get());									//Write all unpackable nodes to xml file
}

void PackingTracker::writeFailedFiles (XMLWriter *xml)
{
	if (m_failedFiles.empty					())											//Do not process if no fails exist
		return;

	DirectoryFileMapIter		dfi		=	m_failedFiles.begin();
	FilesIter					fi		=	(*dfi).second.begin();
	boost::shared_ptr <Devil>	img			(new Devil());


	
	//vars used to write data to XML file
    vector <string>     tags;
    vector <string>     elems;

    stringstream        ss;

	for (;	dfi	!= m_failedFiles.end(); dfi++)
	{
		ss.str							("");
		tags.clear						();
		elems.clear						();

		for (; fi != (*dfi).second.end(); fi++)
		{
			img->Load						((*fi));

			tags.push_back                  ("IMG_SRC");
			tags.push_back                  ("IMG_DEST");
			tags.push_back                  ("X_OFFSET");
			tags.push_back                  ("Y_OFFSET");
			tags.push_back                  ("WIDTH");
			tags.push_back                  ("HEIGHT");
			tags.push_back                  ("TEXTURE");

			elems.push_back                 (img->getFile());
			elems.push_back                 (img->getFile());

			ss.str                          ("");
			ss                  <<          0;
			elems.push_back                 (ss.str());

			ss.str                          ("");
			ss                  <<          0;
			elems.push_back                 (ss.str());

			ss.str                          ("");
			ss                  <<          img->m_img->getImageWidth();
			elems.push_back                 (ss.str());

			ss.str                          ("");
			ss                  <<          img->m_img->getImageHeight();
			elems.push_back                 (ss.str());

			xml->WriteGroup(tags, elems);
		}
	}
}

DirectoryNodes PackingTracker::mergeDirectories(BIN_SIZE mergeSize)
{
	DirectoryNodes	nodes	=	findMergables(m_nodes);							//Initialize merge candidate list
	DirectoryNodesIter i	=	nodes.begin();
	bool			merged	=	true;
	
	while (merged)
	{
		merged				=	false;											//Turn off flag

		for (i	= nodes.begin(); i != nodes.end(); i++)							//Loop through potential list
		{
			if (mergeNodes(i, mergeSize))										//Reset flag if merge succeeded
				merged		=	true;
						
		}		

		nodes			=	findMergables(nodes);								//Move directories up one level
	}

	return nodes;

	
}

DirectoryNodes	PackingTracker::findMergables (DirectoryNodes sourceNodes)
{
	DirectoryNodes				mergeCandidates;								//Node list to be filled
	DirectoryNodesIter			x	=	sourceNodes.begin(),					//Source node list
								i;
	string						parent;											//Holds the parent directory of a given node
	Node						siblings;										//The list of node data that will be merged into 
																				// map
	//Create map of all sibling nodes
	for (; x != sourceNodes.end(); x++)
	{
		if	((*x).first			==		BinPacker::m_imgDir)					//Do not merge passed root directory
			parent				=		BinPacker::m_imgDir;
		else
			parent				=		Directory::getParentDir((*x).first);	//Get parent directory of node

		i						=		mergeCandidates.find (parent);			//See if key already exists in map

		if	(i					==		mergeCandidates.end())
			mergeCandidates.insert			(make_pair(parent, (*x).second));	//Doesn't exist. Create new merge candidate
		else
			(*i).second.merge				((*x).second);						//Key exists. Merge the two node lists
	}

	return mergeCandidates;
}

bool PackingTracker::mergeNodes	(DirectoryNodesIter i, BIN_SIZE mergeSize)
{
	ImagePacker							*temp,													//Pushed into list if succeeded
										*head;
	boost::shared_ptr<Devil>			img			(new Devil());								//Temp pointer used to load files
	NodeIter							ti		=	(*i).second.begin(),						//Iterators used in loop
										ni;
	long								areaHead,												//Area of ti node
										areaMerge,												//Area of the potential merge node
										occupArea;												//Total occupied area between two nodes
	bool								packed;													//Used within loop to reset ti
	bool								oneDirectoryPacked	=	false;							//Used to determin if another iter is			
																								// needed
	while (ti != (*i).second.end())
	{
		temp							=		(*ti).get();
		head							=		temp;
		areaHead						=		temp->getArea();
		ni								=		(*i).second.begin();

		while (ni != (*i).second.end())															//Loop through all mergable nodes
		{
			packed						=		false;
			areaMerge					=		(*ni)->getArea();
			
			if ((*ni)					==		(*ti))											//Do not pack duplicat node
				ni++;
			else
			{
				occupArea				=		(*ni)->getOccupiedArea((*ni).get(), img.get()) + //Get total occupied area after merge
												temp->getOccupiedArea(temp,	img.get());

				if (areaHead			==		areaMerge &&									//See if new node will fit
					occupArea			<=		areaHead)
				{
					temp				=		temp->mergeTree	(temp, (*ni).get());			//Merge nodes

					if (temp			!=		NULL)											//Check if merge succeeded
					{
						NodeIter x		=		ni;												//Delete old node
						ni++;	
						(*i).second.erase		(x);
						(*i).second.erase		(ti);
						(*i).second.push_back	(boost::shared_ptr<ImagePacker>(temp));			//Push new pointer into stack
						packed			=		oneDirectoryPacked = true;						//Indicate that a pack succeeded
						
					}
					else
						ni++;

					temp				=		head;
				}
				else
					ni++;
			}
		}

		if (packed)																				//Restart or increment outside iterator
			ti							=		(*i).second.begin();
		else
			ti++;
	}
	
	return oneDirectoryPacked;
	
}