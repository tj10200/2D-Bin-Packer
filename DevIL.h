#ifndef DEVIL_H
#define DEVIL_H

#include "commonHeaders.h"
#include "Image.h"

class Devil
{

private:
    ILuint              m_imgID;                //Holds the Image ID associated with OpenIL
    static bool         m_initialized;          //Tells the class objects if DevIL is initialized
    void                printError      ();     //Prints the error that happened to the screen
	

public:
    boost::shared_ptr<Image> m_img;					//Holds data related to the image itself
	
	Devil                               ();
    Devil                               (string fileNm);    //Loads a file at construction
    Devil                               (string fileNm,     //Creates a new file at construction
                                         ILuint width,
                                         ILuint height);
    ~Devil                              ();
    void                Init            ();
    ILboolean           Load            (string fileNm);    //Load an image into active buffer
    bool                Load            (ILuint imgID,      //Load an image into active buffer
                                         string fileNm);
    ILboolean           Save            (string fileNm);    //Save the active image
    ILboolean           Create          (string fileNm,     //Create file
                                         ILuint width,
                                         ILuint height);
    bool                Merge           (ILuint srcImgID,   //Source Image
                                         int    xOffset,    //offset to copy into dest image
                                         int    yOffSet);
	bool				Delete			(string fileNm);
    ILuint              getImgID        ()              {return m_imgID;}
    string              getFile         ()              {return m_img->getFilePath();}


};



#endif
