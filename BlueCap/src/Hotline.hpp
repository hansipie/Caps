#define CONF_FILE "./conf/ServConf.conf"

//
// Default config
//

#define DLFT_PORT	5500
#define DLFT_NAME	"Caps Server: BleuCap"
#define DLFT_AGREEM	"./conf/agreement.txt"
#define DLFT_FILESFLD	"./files/"
#define DLFT_FILEST	"./conf/filestype.conf"
#define ACCOUNTS	"./accounts/"
#define RECYCLEBIN	"./Trash/"
#define VLCPICT		"./conf/vlc.bmp"
#define VERSION		190

//
// Transaction initialisation
//

#define HAND_DFLT	"TRTP"
#define HAND_CAPS	"CAPS"
#define HAND_HOTL	"HOTL"

#define DUHAND_DFLT	"HTXF"
#define DUHAND_CAPS	"CPXF"


//
// Values defines
//

#define HLHDR_SIZE	20
#define HDR_FLAG	0
#define HDR_RPLY	1
#define HDR_RQST	0

#define DIE		-1

#define T_CONN		(1<<0)
#define T_READ		(1<<1)
#define T_WRITE		(1<<2)
#define T_FREE		(1<<3)

#define T_BP		1
#define T_BP1		2
#define T_BP2		3
#define T_BP3		4

#define DWL		1
#define PRV		2
#define UPL		3
#define WAIT		4
#define PREVWAIT	5
#define END		6

#define BUFFSIZE	65536
//#define BUFFSIZE	4096
#define TIMEOUT		100
#define MAXID		20
#define BACKLOG		10 // how many pending connections queue will hold
#define UNLIMIT		0 // turn on or off fd limit

#define HANDSHAKE	0
#define ANY		-1

#define HANDSHAKE_REQ           0
#define HANDSHAKE_REP           1
#define TRANS_LOGIN_REQ         2
#define TRANS_LOGIN_REP         3
#define TRANS_SHOW_AGREE        4
#define TRANS_AGREED            5
#define TRANS_USRNAMELST_REQ    6
#define TRANS_USRNAMELST_REP    7
#define TRANS_FILESNEWSLST_REQ  8
#define TRANS_FILESNEWSLST_REP  9

#define SESSION_IN	0
#define SESSION_OUT	1

#define BIN		1
#define LINT		2 
#define SINT		3
#define STRG		4
#define MAXUINT16	65536



//
// Transaction types
//

#define NO_TYPE		0
#define HL_ERROR	100
#define GET_MSG		101
#define NEW_MSG		102
#define OLD_POST	103
#define SERV_MSG	104
#define SEND_CHAT	105
#define CHAT_MSG	106
#define	LOGIN		107
#define SEND_MSG	108
#define SHOW_AGREEM	109
#define DISC_USER	110
#define DISC_MSG	111
#define INVIT_TO_NEW_CHAT	112
#define INVIT_TO_CHAT	113
#define JOIN_CHAT	115
#define LEAVE_CHAT	116
#define SET_CHAT_SUBJECT	120
#define AGREED		121
#define GET_FNAME_LST	200
#define DWNL_FILE	202
#define UPL_FILE	203
#define DEL_FILE	204
#define NEW_FOLD	205
#define GET_FILEI	206
#define SET_FILEI	207
#define MOVE_FILE	208
#define MK_FILE_ALIAS	209
#define DWNL_FOLD	210
#define DWNL_BAN	212
#define UPL_FOLD	213
#define GET_UNAME_LST	300
#define NOT_CHANGE_USR	301
#define NOT_DEL_USR	302
#define GET_CL_INF_TXT	303
#define SET_CL_USR_INF	304
#define GET_ALL_ACCOUNTS	348
#define SAVE_ALL_ACCOUNTS	349
#define NEW_USER	350
#define DEL_USER	351
#define GET_USER	352
#define SET_USER	353
#define USER_ACCES	354
#define USER_BROADCAST  355
#define GET_NEWS_CAT_NAME_LST	370
#define GET_NEWS_ART_NAME_LST	371
#define DEL_NEWS_IT	380
#define NEW_NEWS_FOLD	381
#define NEW_NEWS_CAT	382
#define GET_NEWS_ART_DATA	400
#define POST_NEWS_ART_DATA	410
#define DEL_NEWS_ART	411

//
// Transaction fields
//

#define ID_END_FIELD	0
#define ID_ERROR	100
#define ID_DATA		101
#define ID_USR_NAME	102
#define ID_UID		103
#define ID_USR_ICON	104
#define ID_USR_LOG	105
#define ID_USR_PASS	106
#define ID_REF_NBR	107
#define ID_TRANS_SIZE	108
#define ID_CHAT_OPT	109
#define ID_USR_ACCESS	110
#define ID_USER_FLAG	112
#define ID_OPT		113
#define ID_CHAT_ID	114
#define ID_WAIT_COUT	116
#define ID_BANNER	151
#define ID_BANNER_TYPE	152
#define ID_BANNER_URL	153
#define ID_NO_AGREEM	154
#define ID_VER		160
#define ID_BANNER_ID	161
#define ID_SERV_NAME	162
#define ID_FILE_NAME_INFO	200
#define ID_FILE_NAME	201
#define ID_FILE_PATH	202
#define ID_FILE_RESUME	203
#define ID_FILE_TRANS_OPT	204
#define ID_FILE_TYPE_STR	205
#define ID_FILE_CREA_STR	206
#define ID_FILE_SIZE	207
#define ID_FILE_CREA_DAT	208
#define ID_FILE_MOD_DAT	209
#define ID_FILE_COMM	210
#define ID_FILE_NEWNAME	211
#define ID_FILE_NEWPATH	212
#define ID_FILE_TYPE	213
#define ID_QUOT_MSG	214
#define ID_AUTO_RESP	215
#define ID_USR_NAME_INFO	300

//
// Transaction fields
//

#define RPM	" refuse private message"
