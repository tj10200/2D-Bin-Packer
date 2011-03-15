#include "DevIL.h"

bool Devil::m_initialized = false;
Devil::Devil()
{	
	m_img.reset				();
	m_imgID				=	0;
	Init();
}
Devil::Devil(string fileNm)
{
	m_img.reset				();
	m_imgID				=	0;
	Init();
    Load(fileNm);
}

Devil::Devil(string fileNm, ILuint width, ILuint height)
{
	m_img.reset();
	m_imgID				=	0;

	//Initialize IL environment
    Init();

    //Create new file
    Create                  (fileNm,
                             width,
                             height);

    //Save results
    Save                     (fileNm);
}

Devil::~Devil()
{	
	if (m_imgID			!=	0)	
		ilDeleteImages(1, &m_imgID);
}
void Devil::Init()
{

    if (!m_initialized)
    {
        cout            <<  "DevIL environment initialized successfully\n";
        ilInit();

         //Set origin to top left
        ilEnable    (IL_ORIGIN_SET);

	   ilClearColour (0,0,0,1);

        Devil::m_initialized = true;
    }
    else
        cout            <<  "DevIL environment already initialized\n";
}

ILboolean Devil::Load(string fileNm)
{
	ilEnable                (IL_ORIGIN_SET);    //Use a standard top left origin

	if(m_imgID			!=	0)
		ilDeleteImages		(1, &m_imgID);

    m_imgID             =   ilGenImage  ();     //Get a new image ID
	

    cout                <<  "Loading image\n";
    ilBindImage             (m_imgID);          //Activate the created image

    ILboolean	result	=   ilLoadImage (fileNm.c_str());//Load the image

    //Print out result of load
    if (result)
    {
        cout            << "Image loaded successfully\n";
        
		m_img.reset			(new Image(fileNm));

		m_img->repopImageData();

    }
    else
        printError();

    return result;
}

bool Devil::Load(ILuint imgID, string fileNm)
{

    m_imgID             =   imgID;              //Reset the image ID

    cout                <<  "Loading image\n";
    ilBindImage             (m_imgID);          //Re-Activate the image

    //Check if image was loaded
    ILenum      err     =   ilGetError();
    if (err             ==  IL_ILLEGAL_OPERATION)
    {
        //Try to load by file name only
        if (!Load(fileNm))
            return false;
    }
    else
    {
        cout            <<  "Image loaded successfully";
		m_img.reset			(new Image(fileNm));
    }

	return true;
}


ILboolean Devil::Save(string fileNm)
{
    //Activate current image
    ilBindImage             (m_imgID);

    cout                <<  "Saving file: "
                        <<  fileNm
                        <<  "\n";

    ilEnable                (IL_FILE_OVERWRITE);
    //Save to the file
    ILboolean	result	=   ilSaveImage(fileNm.c_str());

    //Check if error encountered
    if (result)
    {
        if (m_img		==	NULL)
			m_img.reset		(new Image(fileNm));
		
		cout            <<  "The file was saved successfully.\n";
    }
    else
        printError();

	
    return result;
}

ILboolean Devil::Create(string fileNm, ILuint width, ILuint height)
{
    cout                <<  "A new image is being created\n";

	if(m_imgID			!=	0)
		ilDeleteImages		(1, &m_imgID);

    //Create a new IL image ID of size 1x1
    m_imgID             =   ilGenImage();
	
    //Activate image
    ilBindImage             (m_imgID);

	m_img.reset				(new Image(fileNm));

	if (m_img			!=	NULL)
		cout			<<	"Image Data Populated\n";

    ilClearColour			(0,0,0,0);

	boost::shared_ptr<ILubyte> b		((ILubyte *)malloc(width * height * 4 * sizeof(unsigned int)), free);
    
	//Enlarge the image to meet the parameters specs
    ILboolean   result  =   ilTexImage ( width,             //Image width,
                                         height,            // image height
                                         1,                 // depth
                                         4,                 // Channels per image
                                         IL_RGBA,            // Image format
                                         IL_UNSIGNED_BYTE,  // Image data format
                                         b.get()					// Image byte data pointer
										);

	//Advise of status
    if (result)
    {
		ilClearImage();

		cout				<<			fileNm 
							<<			" created successfully.\n";

		Save							(fileNm);			//Save the newly created file
    
		m_img->repopImageData			();					//Ensure that image has valid image data
	}
    else
        printError						();

    return result;
}
bool Devil::Merge(ILuint srcImgID, int destXOffset, int destYOffset)
{
    //Activate the source image
    ilBindImage             (srcImgID);

    //populate image data memebers
    auto_ptr<ImageData> img (new ImageData());
    
	img->setImageData();


    //Check if image ID was lost
    if (m_imgID         ==  0)
    {
        cout            <<  "Invalid image being processed\n"
                        <<  "Returning.\n";
        return              false;
    }

    //Activate destination image
    ilBindImage             (m_imgID);

    cout                <<  "Copying source data to destination image\n";

	//Set pixels in destination
    ilSetPixels             (destXOffset,
                             (m_img->getImageHeight() - img->getHeight()) - destYOffset,
                             0,
                             img->getWidth(),
                             img->getHeight(),
                             1,
                             IL_RGBA,
                             IL_UNSIGNED_BYTE,
                             img->getBytes()
                            );

    


    //Save File
    Save(m_img->getFilePath());

	return true;
}

void Devil::printError()
{
    //Advise that an error was encountered
    ILenum      err     =  ilGetError();

    cout                << "The following error was encountered: "
                        << err
                        << "\n"
                        << ilGetString(err)
                        << "\n";
    return;
}
