class CRep;

class CFileList
{
public:	
	
	CFileList(void);
	~CFileList(void);

	CRep		*parent;
	wxString	Filename;
	wxString	size;
	wxString	FileType;
	wxString	Creator;
};
