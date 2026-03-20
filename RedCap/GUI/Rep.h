class CFileList;

class CRep
{
public:	
	CRep(void);
	~CRep(void);

	vector <CRep *>			sous_rep;
	vector <CFileList *>	fichier;
	CRep					*parent;
	wxString				nom;
	wxString				size;

	
};
