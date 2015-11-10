#ifndef OSDCHAR_INCLUDED
#define OSDCHAR_INCLUDED

#include <string.h>

#define CODE_12 0x02
#define CODE_16 0x03
#define CODE_24 0x04
#define CODE_32 0x05
#define GB2312_LOCATE(qh,wh) 94*((qh)-1)+(wh-1)
typedef enum VIDEO_FORMAT{YUV_420SP=0,YUV_UYVY} Video_format ;
enum tagENUM{ENGLISH_CHARSET=1,CHINESE_CHARSET} ;

typedef struct tagYUVIMAGE
{
   Video_format videoformat ;
   unsigned short int uiYUVWidth ;
   unsigned short int uiYUVHeight ;
   unsigned char* p420SPY ;
   unsigned char* p420SPUV ;
}sYuvImage ;

typedef struct tagFONTTABLE
{
    unsigned char ucHZCharWidth ;
    unsigned char ucHZCharHeight ;
    unsigned char ucASCCharWidth ;
    unsigned char ucASCCharHeight ;
    const unsigned char* pHZKSrc ;  //chinese code table
    const unsigned char* pASCKSrc ; //ascii code table
}sMyFontTable ;


typedef struct tagFONT
{
    unsigned short int ucFlage ; //1 字符 2 汉字
    unsigned short int ucPoscolumn ; // ==column
    unsigned short int ucPosrow ; // ==row
    unsigned char ucCharWidth ;
    unsigned char ucCharHeight ;
    unsigned int uiMapAddress ;  //osd char ;
}sMyFont ;

typedef struct tagOSDPOS
{
    unsigned short int ucFlag ; //字符还是图形 1表示字符，此时ucEndX，ucEndY无效 2表示图形 4个参数全部有效
    unsigned short int ucStartX ; // ==column
    unsigned short int ucStartY ; // ==row
    unsigned short int ucEndX ; // ==column
    unsigned short int ucEndY ; // ==row
}sOsdPos ;




extern char OSDCharYUV422(int column, int row, int imageWidth, int imageHeight, char ascii , char *buffer) ;    //column 列 row 行
/*******************************************************************************
* Function Name  : FrontSetup
* Description    : Setup a Front LIB on OSD.
* Input          : - FontID: This parameter can be one of the following values:
*                    :
* Output         : None
* Return         : None
*******************************************************************************/
extern void FrontSetup(char FontID) ;
//----------------------------------------------------------------------------
extern void OSDSetTextColor(char OsdR, char OsdG, char OsdB) ;


static char DrawChar2OSD420SP(const sMyFont *pMyFont, sYuvImage *pImage) ; //把指定的字符写到osd上
static char DrawChar2OSDUYVY(const sMyFont *pMyFont, sYuvImage *pImage) ; //把指定的字符写到osd上

extern char NEWOSDCharUYVY(const unsigned char *pString, const sOsdPos* pOsdPos,
       sYuvImage *pImage) ;    //column 列 row 行
extern char NEWOSDCharYUV420SP(const unsigned char *pString, const sOsdPos* pOsdPos,
       sYuvImage *pImage) ;    //column 列 row 行

extern char DrawCrossLine( const sOsdPos horLinePoint,const sOsdPos verLinePoint,const int iLineWdith, sYuvImage *pImage ) ;

extern char DrawHorLine( const sOsdPos sPoint,const int iLineWdith, sYuvImage *pImage ) ;

extern char DrawVerLine( const sOsdPos sPoint,const int iLineWdith, sYuvImage *pImage ) ;


#endif // OSDCHAR_INCLUDED
