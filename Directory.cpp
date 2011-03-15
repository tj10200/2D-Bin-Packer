#include "Directory.h"


bool  Directory::getDirTree()
{

    HANDLE                      hFind       =   INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA             ffd;
    string                      spec;
    stack<string>               directories;

    //Push Root directory
    directories.push            (m_rootDir);

    while (!directories.empty())
    {
        //Recapture root directory
        m_rootDir           =   directories.top();

        //Assign spec to be rootDir\\*
        spec                =   m_rootDir				+   "\\"	+   m_mask;
        
        directories.pop();

        //Get the first file within the root directory
        hFind               =   FindFirstFile((LPCTSTR)spec.c_str(), &ffd);

        //If no files then return false
        if (hFind == INVALID_HANDLE_VALUE)
            return false;

        //Loop through all files in current directory
        do
        {
            //make sure file is not the parent or current directory
            if (strcmp(ffd.cFileName, ".")     != 0 &&
                strcmp(ffd.cFileName, "..")    != 0 &&
                strcmp(ffd.cFileName, "bins")  != 0)
            {
                cout << ffd.cFileName << "\n";
                //Check if is a file or a directory
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
                    directories.push		(m_rootDir + "\\" + ffd.cFileName);
					m_directories.push_back	(m_rootDir + "\\" + ffd.cFileName);
				}
				else
                    m_files.push_back   (m_rootDir + "\\" + ffd.cFileName);
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        //Check if at last file in directory
        if  (GetLastError() != ERROR_NO_MORE_FILES)
        {
            FindClose(hFind);
            return false;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return true;
}

list<string> Directory::getDirs (string rootDir)
{
	HANDLE                      hFind       =   INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA             ffd;
    string                      spec;
    stack<string>               directories;
	list<string>				dirs;


    //Push Root directory
    directories.push            (rootDir);

    while (!directories.empty())
    {
        //Recapture root directory
        rootDir				=   directories.top();

        //Assign spec to be rootDir\\*
        spec                =   rootDir				+   "\\*.*";
        
        directories.pop();

        //Get the first file within the root directory
        hFind               =   FindFirstFile((LPCTSTR)spec.c_str(), &ffd);

        //If no files then return false
        if (hFind == INVALID_HANDLE_VALUE)
            return dirs;

        //Loop through all files in current directory
        do
        {
            //make sure file is not the parent or current directory
            if (strcmp(ffd.cFileName, ".")		!= 0 &&
                strcmp(ffd.cFileName, "..")		!= 0)
            {
                cout << ffd.cFileName << "\n";
                //Check if is a file or a directory
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
                    directories.push			(rootDir + "\\" + ffd.cFileName);
					dirs.push_back				(rootDir + "\\" + ffd.cFileName);
				}
			}
        } while (FindNextFile(hFind, &ffd)		!= 0);

        //Check if at last file in directory
        if  (GetLastError() != ERROR_NO_MORE_FILES)
        {
            FindClose(hFind);
            return dirs;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

	//Sort and remove dups
	dirs.sort();
	dirs.unique();

    return dirs;
}

list<string> Directory::getFiles (string rootDir, string mask)
{
	HANDLE                      hFind       =   INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA             ffd;
    string                      spec;
    stack<string>               directories;
	list<string>				files;


    //Push Root directory
    directories.push            (rootDir);

    while (!directories.empty())
    {
        //Recapture root directory
        rootDir				=   directories.top();

        //Assign spec to be rootDir\\*
        spec                =   rootDir				+   "\\"	+ mask;
        
        directories.pop();

        //Get the first file within the root directory
        hFind               =   FindFirstFile((LPCTSTR)spec.c_str(), &ffd);

        //If no files then return false
        if (hFind == INVALID_HANDLE_VALUE)
            return files;

        //Loop through all files in current directory
        do
        {
            //make sure file is not the parent or current directory
            if (strcmp(ffd.cFileName, ".")		!= 0 &&
                strcmp(ffd.cFileName, "..")		!= 0)
            {
                cout << ffd.cFileName << "\n";
                //Check if is a file or a directory
                
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				   files.push_back				(rootDir + "\\" + ffd.cFileName);
			}
        } while (FindNextFile(hFind, &ffd)		!= 0);

        //Check if at last file in directory
        if  (GetLastError() != ERROR_NO_MORE_FILES)
        {
            FindClose(hFind);
            return files;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

	//Sort and remove dups
	files.sort();
	files.unique();

    return files;
}

list<string> Directory::getFilesRecur (string rootDir, string mask)
{
	HANDLE                      hFind       =   INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA             ffd;
    string                      spec;
    stack<string>               directories;
	list<string>				files;


    //Push Root directory
    directories.push            (rootDir);

    while (!directories.empty())
    {
        //Recapture root directory
        rootDir				=   directories.top();

        //Assign spec to be rootDir\\*
        spec                =   rootDir				+   "\\"	+ mask;
        
        directories.pop();

        //Get the first file within the root directory
        hFind               =   FindFirstFile((LPCTSTR)spec.c_str(), &ffd);

        //If no files then return false
        if (hFind == INVALID_HANDLE_VALUE)
            return files;

        //Loop through all files in current directory
        do
        {
            //make sure file is not the parent or current directory
            if (strcmp(ffd.cFileName, ".")		!= 0 &&
                strcmp(ffd.cFileName, "..")		!= 0)
            {
                cout << ffd.cFileName << "\n";
                //Check if is a file or a directory
				cout << ffd.cFileName << "\n";
                //Check if is a file or a directory
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				     directories.push			(rootDir + "\\" + ffd.cFileName);
				else
				   files.push_back				(rootDir + "\\" + ffd.cFileName);
			}
        } while (FindNextFile(hFind, &ffd)		!= 0);

        //Check if at last file in directory
        if  (GetLastError() != ERROR_NO_MORE_FILES)
        {
            FindClose(hFind);
            return files;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

	//Sort and remove dups
	files.sort();
	files.unique();

    return files;
}

bool Directory::CreateNewDirectory(string rootDir, string path)
{
    string  fullPath = rootDir + path;

    CreateDirectory(fullPath.c_str(), NULL);

    if (GetLastError() == ERROR_PATH_NOT_FOUND)
    {
        cout << "path not found\n";
        return false;
    }
    else
        return true;

}



bool Directory::CreateNewDirectory(string path)
{
    CreateDirectory(path.c_str(), NULL);

    if (GetLastError() == ERROR_PATH_NOT_FOUND)
    {
        cout << "path not found\n";
        return false;
    }
    else
        return true;

}

string Directory::getParentDir (string path)
{
	size_t	len		=	path.length();
	LPSTR	temp	=	new char[len + 1];
	
	path._Copy_s(temp,len,len);
	temp[len]='\0';

	PathRemoveFileSpec(temp);

	return (string)temp;


}
