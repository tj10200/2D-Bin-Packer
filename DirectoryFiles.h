
struct Directories 
{
	string	m_curDir;
	list<string> m_files;
	Directories *m_next;
	Directories *m_prev;
};