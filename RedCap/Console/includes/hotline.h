#define DEBUG 1

#define STRG 1
#define LINT 2
#define SINT 3
#define BIN 4

#define TIMEOUT 100 /* millisecond */
#define BUFF_SZE 4096
#define BIG_BUFF 4096

#define MAXUINT16 65536

#define DICE_TIME 5000000
#define DICE_TIMEOUT  10000000
#define INCR		1.2

#define MAX_LOOSE 3
#define NOT_DICE 0
#define WIN_STR "gagne"
#define LOOSE_STR "perdu"
#define NUL_STR "Match Nul"
#define NUL_NBR 1000000000

#define LOCAL_PATH "./download/"
#define VLCPATH "/usr/bin/vlc"

//
// Header defines
//

#define HLHDR_SIZE 20
#define HDR_FLAG 0
#define HDR_RPLY 1
#define HDR_RQST 0

//
// Transaction types
//

#define NO_TYPE			0
#define GET_MSG			101
#define SERV_MSG		104
#define SEND_CHAT		105
#define RCV_CHAT_MSG		106
#define	LOGIN			107
#define SEND_MSG		108
#define SHOW_AGREEM		109
#define INVIT_TO_NEW_CHAT	112
#define INVIT_TO_CHAT		113
#define JOIN_CHAT		115
#define LEAVE_CHAT		116
#define SET_CHAT_SUBJECT	120
#define AGREED			121
#define GET_FNAME_LST		200
#define FILE_NAME		201
#define DWNL_FILE		202
#define FILE_PATH		202
#define UPL_FILE		203
#define DELETE_FILE	       	204
#define MKDIR			205
#define GET_FILEI		206
#define SET_FILE_INFO		207
#define MOVE_FILE		208
#define DWNL_FOLD		210
#define DWNL_BAN		212
#define UPL_FOLD		213
#define GET_UNAME_LST		300
#define GET_CL_INF_TXT		303
#define SET_CL_USR_INF		304
#define NEW_USR     		350
#define DELETE_USR     		351
#define GET_USR			352
#define SET_USR                 353
#define USER_ACCES		354
#define SEND_BROADCAST		355
#define GET_NEWS_CAT_NAME_LST	370
#define GET_NEWS_ART_NAME_LST	371
#define GET_NEWS_ART_DATA	400

//
// Transaction fields
//

#define ID_END_FIELD		0
#define ID_DATA			101
#define ID_USR_NAME		102
#define ID_UID			103
#define ID_USR_ICON		104
#define ID_USR_LOG		105
#define ID_USR_PASS		106
#define FILE_TRANSFERT_SIZE	108
#define ID_CHAT_OPT		109
#define ID_USR_ACCESS		110
#define ID_OPT			113
#define ID_BANNER		151
#define ID_BANNER_TYPE		152
#define ID_BANNER_URL		153
#define ID_NO_AGREEM		154
#define ID_VER			160
#define ID_BANNER_ID		161
#define ID_SERV_NAME		162
#define FILE_RESUME_DATA	203
#define ID_FILE_TRANSFERT_OPT	204
#define FILE_NEW_NAME		211
#define ID_AUTO_RESP		215
#define ID_USR_NAME_INFO	300

/* #define HDR_TYPE 1 */
/* #define HDR_ID	4 */

/* #define REPLY	1 */
/* #define REQUEST 0 */

/* #define	LOGIN	(u_int16_t)107 */
/* #define AGREED  (u_int16_t)121 */
/* #define SET_USER_INFO (u_int16_t)304 */
/* #define GET_USER_LIST (u_int16_t)300 */

/* #define RCV_SV_MSG	104 */
/* #define SEND_CHAT	105 */
/* #define RCV_CHAT_MSG	106 */
/* #define SEND_MSG	108 */
/* #define SHOW_AGREEM	109 */
/* #define SEND_AGREED	121 */
/* #define SEND_BROADCAST  355 */
/* #define DISCONNECT_USER 110 */

/* #define ID_END_FIELD	0 */
/* #define ID_DATA		101 */
/* #define ID_USR_NAME	102 */
/* #define ID_UID		103 */
/* #define ID_USR_ICON	104 */
/* #define ID_USR_LOG	105 */
/* #define ID_USR_PASS	106 */
/* #define ID_CHAT_OPT	109 */
/* #define ID_OPT		113 */
/* #define ID_USR_VER	160 */
