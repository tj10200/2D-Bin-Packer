#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "commonHeaders.h"

class XMLWriter
{
private:
    string          m_filePath;
    ofstream        m_file;
    bool            openFile        ();
public:
    XMLWriter                       (string filePath);
    void            WriteGroup      (vector<string> tags,
                                     vector<string> elements);
    void            WriteLine       (string         tag,
                                     string         element);

};


#endif
