/////////////////////////////////////
// 3DMM Pencil++ 2 Messages Header //
/////////////////////////////////////

typedef unsigned long quad;
typedef unsigned char BYTE;
struct SExtQuadID
{
	quad type;
	unsigned long id;
};
//struct SExtString
//{
//	wchar_t *string;
//	int stringtype;
//	unsigned short stringlen;
//}
//struct SExtSection
//{
//	BYTE *data;
//	size_t datalen;
//	bool compressed;
//	
//}
//
//struct SExtQuad
//{
//	SExtQuadID quadid;
//	SExtString string;
//}

#define MSG_GETCURQUADID				0
#define MSG_GETCURQUADSTRING			1
#define MSG_GETCURQUADSTRINGLEN			2
#define MSG_ALLOCATEBYTEARRAY			3
#define MSG_SAVECURQUADSECTION			4