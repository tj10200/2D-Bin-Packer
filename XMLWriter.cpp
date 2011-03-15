#include "XMLWriter.h"

XMLWriter::XMLWriter (string filePath)
{
    m_filePath          =   filePath;
    openFile                ();

}
bool XMLWriter::openFile ()
{
    m_file.open(m_filePath.c_str());

    if (m_file.good())
        m_file  << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";

    return m_file.good();

}

void XMLWriter::WriteGroup (vector<string> tags, vector<string> elements)
{
    //Collect the group tag to use at closing
    string  groupTag    =   tags.back();
    m_file              <<  "<" + groupTag + ">\n";

    tags.pop_back           ();

	vector<string>::iterator i;
	vector<string>::iterator v;

    for (i = tags.begin(), v = elements.begin();
         i != tags.end() &&
         v != elements.end();
         i++,
         v++)
    {
        WriteLine            ((*i), (*v));
    }

    m_file              <<   "</"  + groupTag + ">\n";

    return;
}

void XMLWriter::WriteLine (string tag, string element)
{

    m_file          <<  "\t"
                    <<  "<"     + tag  + ">"
                    <<  element
                    <<  "</"    + tag  + ">\n";


    return;
}

