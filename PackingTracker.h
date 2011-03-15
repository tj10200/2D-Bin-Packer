#ifndef PACKING_TRACKER_H
#define PACKING_TRACKER_H

#include "BinPacker.h"

typedef list<pair<string, unsigned int> >				FileHeights;
typedef FileHeights::iterator							FileHeightsIter;
typedef	map<string, FileHeights>						FileMapHeights;
typedef FileMapHeights::iterator						FileMapHeightsIter;
typedef map<string, bool>								PackedDirectory;

const long MAX_PACKABLE_AREA					=		700 * 700;

class PackingTracker: public BinPacker
{
private:
		PackedDirectory			m_completeDirs;									//Holds a structure of all parent directories and if they were packed
		DirectoryFileMap		m_failedFiles;									//Holds all files that were unable to pack completely							
		FileMapHeights			m_fileMapHeights;								//Same as Directory File map with image heights added to list
		
		void					initMaps				();						//Populated the maps with data and initializes flags
		void					initFileHeights			();						//Loop through entire map and get file heights
		void					sortFileHeights			();
		void					reinitDirFileMap		();						//Repopulate the directory file map with sorted lists
		void					sortDirFileMap			();
		bool					packDirectory			(DirectoryFileMapIter	i,//Packs one directory into a node of specified size
														 BIN_SIZE				b);

		DirectoryNodes			mergeDirectories		(BIN_SIZE			mergeSize);//Merges subdirectories into single image
		void					writeBins				(DirectoryNodes		nodes);//Merges all images and writes all node data to log file
		void					writeFailedFiles		(XMLWriter		*	xml);//Writes all unmergable images to log file
		void					cleanNodes				();						//Deletes any zero sized nodes
		Files					sortFileList			(Files				f, 
														 Devil				*imgLoader);

		DirectoryNodes			findMergables			(DirectoryNodes		sourceNodes);
		bool					mergeNodes				(DirectoryNodesIter i,
														 BIN_SIZE			mergeSize);

public:
								PackingTracker			(string				imgDir,
														string				binDir,
														unsigned int		padding, 
														string				logFile);
		virtual					~PackingTracker			(){}

		virtual	void			 packImages				();



};


#endif