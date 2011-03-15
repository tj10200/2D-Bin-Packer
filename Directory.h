#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "commonHeaders.h"
#include "Shlwapi.h"

class Directory
{
protected:
    vector<string>		m_files;
	vector<string>		m_directories;
    string				m_rootDir;
    string				m_mask;
    bool				getDirTree      ();
	
public:
    Directory							(string    startDirectory,
                                         string    mask)
    {
        m_rootDir   =   startDirectory;
        m_mask      =   mask;

        getDirTree                      ();

    }

	virtual	~Directory					(){}
    vector<string>      getFiles()
    {
        if (!getDirTree  ())
            return      vector<string>(0);
        else
            return      m_files;
    }

    bool			CreateNewDirectory   (string     rootDir,
                                          string     path);
    bool			CreateNewDirectory   (string     path);

    string			getRoot()									{return				m_rootDir;}
    string			getMask()									{return				m_mask;}
    
	void			setRoot(string s)							{m_rootDir		=   s;}
    void			setMask(string s)							{m_mask			=   s;}
    
	void			clearFiles          ()						{m_files.clear();}

	list<string>	getDirs				(string rootDir);				//Fills the vector of directories
	list<string>	getFiles			(string rootDir,				//Fills the vector of files
										 string mask);
	list<string>	getFilesRecur		(string rootDir,
										 string mask);
	string			getParentDir		(string path);



};

#endif
