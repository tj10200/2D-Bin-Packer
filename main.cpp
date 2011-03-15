#undef _UNICODE

#include "PackingTracker.h"
#include "Directory.h"

list<pair<string, bool>>	populateFileList (	vector<string>	f,				//Function creates a unique sorted list of each
												Devil			*d);			// file
bool						checkCommandArgs (	int				args);			//Displays error message if number of commands are not correct
list<pair<string, bool>> 	fileLoop		 (Directory			*dir,			//Loops through files and places them into bins
											  string			binDir,
											  string			logFile);
void						displayStatus	 (list<pair<string, bool>>);		//Displays the number of files processed and lists any that failed

int main(int argc, char *argv[])
{
	//Check if correct number of arguments were entered
    if (!checkCommandArgs(argc))
		return 2;
	
	//auto_ptr<BinPacker>				bp(new BinPacker(argv[1], argv[2], 1, argv[3]));
	shared_ptr<BinPacker>	packs			(new PackingTracker(argv[1], argv[2], 1, argv[3]));
	
	packs->packImages();

    
	cout    << "Finished Packing textures...\n"
			<< "Press any key to continue";
	cin.get();
    return 0;

}

bool checkCommandArgs (	int args)
{
	if (args						<   4)
    {

        cout                        <<  "Command line parameters not valid:\n "
                                    <<  "enter: \n"
                                    <<  "\t1.) root directory for textures\n"
                                    <<  "\t2.) root directory for bins\n"
                                    <<  "\t3.) XML file to output to\n"
									<<	"Press any key to continue...\n";
	   
	   cin.get();
        return                      false;
    }
	else
		return						true;
}
/*
bool compare_size(pair<string, unsigned int> first, pair<string, unsigned int> second)
{
	return first.second > second.second;
}

list<pair<string, bool>> populateFileList (vector<string> f, Devil *d)
{
	list<pair<string, unsigned int>> fileList;
	list<pair<string, bool>>		 sortedFileMap;


	//Load each image into the devil object in order to get the image height
	for (vector<string>::iterator i = f.begin(); i != f.end(); i++)
	{
		d->Load						(*i);

		fileList.push_back			(make_pair((*i),d->m_img->getImageHeight()));
	}

	//Sort each image by height and remove any
	// duplicates that exist.
	fileList.sort(compare_size);
	fileList.unique();
	
	//Map all images into the file map in descending height order before returning. 
	for(list<pair<string, unsigned int>>::iterator i = fileList.begin(); i != fileList.end(); i++)
		sortedFileMap.push_back		(make_pair((*i).first, false));
	
	return sortedFileMap;

}


/*
list<pair<string, bool>> fileLoop(Directory *dir, string binDir, string logFile)
{
	//Create initial Bin and initialize bin packer pointers
    int             curBin          =   1;
    stringstream    ss;
    ss.str                              ("");
    ss                              <<  binDir
                                    <<  "\\Bin"
                                    <<  curBin
                                    <<  ".png";
    
	//The bin will store all of the images loaded into img
	shared_ptr<Devil> bins				(new Devil    (ss.str(), 512, 512));
    shared_ptr<Devil> img               (new Devil    ());
    
	//Temp will be used to trace through the bin packer tree. Head will hold the first node
	ImagePacker		*temp			=	new ImagePacker(bins.get(), 1, logFile);
    ImagePacker		*head			=	temp;

    //Create auto pointer to track temp pointer
    // will delete pointer when scope runs out
	shared_ptr<ImagePacker> cBin          (head);
	
	//Holds the vector containing files included in dir tree
    // Gets all files with png, then with jpg extension
    vector  <string>       fileVec      (dir->getFiles());
	
    list    <pair<string, bool>> fileMap;

    //Map files to boolean flag to indicate if image has been packed
    fileMap							=	populateFileList (fileVec, img.get());
		
	//Set up flags to determin if an image was packed and if an image failed to pack
    bool        imgInserted         =   true;
    bool        imgFailed           =   false;

	
    while (imgInserted)
    {
        imgInserted                 =   false;

        for (list<pair<string, bool>>::iterator i = fileMap.begin(); i != fileMap.end(); i++)
        {
            if (!(*i).second)
            {
				if(img->Load				((*i).first))
					temp            =   temp->InsertImg(head, temp, img.get());
				else
					temp			=	NULL;

                //Check if insert was successful
                if (temp)
                {
                    imgInserted     =   true;
                    (*i).second     =   true;
                }
                else
                    imgFailed       =   true;

                temp                =   head;
            }
        }

        //Indicated that one pack failed,
        // but at least one other succeeded
        if (imgFailed && imgInserted)
        {
            imgFailed               =   false;
            ss.str                      ("");
            ss                      <<  binDir
                                    <<  "\\Bin"
                                    <<  ++curBin
                                    <<  ".png";

            bins->Create                (ss.str(), 512, 512);

            //Recreate Bin Packer object
            if (temp)
            {
                delete temp;
                temp                =   NULL;
                head                =   NULL;
            }

			//reinitialize node tree
			temp					=   new ImagePacker(bins.get(), 1, logFile);
			head					=   temp;

			//Re-establish autopointer to correct location
			cBin.reset                  (head);

        }

    }
	

	if (temp){
		delete temp;
		temp = NULL;
	}

	return fileMap;

}

void displayStatus(list<pair<string, bool>> fileMap)
{
	int numSucc = 0,
		numFail = 0;

	cout							<<	"\n\nAll Failed Images:\n";

	for (list<pair<string, bool>>::iterator i = fileMap.begin(); i != fileMap.end(); i++)
	{
		if ((*i).second)
			numSucc++;
		else
		{
			cout					<<	(*i).first << "\n";
			numFail++;
		}
	}

	cout << "\n\nNUMBER OF IMAGES PACKED: " << numSucc << "\n"
		 << "NUMBER OF IMAGES FAILED: "	<< numFail << "\n";
	
}
*/