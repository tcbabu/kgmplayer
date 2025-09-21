#include "kulina.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "icons.c"

//#define D_ALLFILES 

#define RED 120
#define GREEN 140
#define BLUE 120

  static int FileType=0;
  // 0: All files 1: Media 2: Music  3: Video 4: Images
  static void Update1 ( void *Tmp ) ;
  static int folder1stat = 0 , folder2stat = 0;
  static char Folder [ 300 ] , Home [ 300 ] = "";
  static void *folderimg = NULL;
  static void *fileimg = NULL , *musicimg = NULL , *videoimg = NULL , *cimg = NULL , *tarimg = NULL , *linkimg = NULL , *gzipimg = NULL , *exeimg = NULL , *binaryimg = NULL;
      
  static DIT *T1;
  static DIX *X1;
  static DIY *Y1 , *Y2;
  typedef char *(*GetType)(char *);
  static char *kgCheckFileType ( char *name );
  static char *kgCheckMediaFile ( char *name );
  static char *kgCheckImageFile ( char *name );

  GetType TypeFun[5]= {
     &kgCheckFileType,
     &kgCheckMediaFile,
     NULL,
     NULL,
     &kgCheckImageFile
  };
#define kgFree(pt) {if(pt!=NULL) free(pt);pt=NULL;}
  int uiSetFileType(int type) {
	  FileType = type;
	  return type;
  }

static int SetThumbNails(ThumbNail **th) {
    int i=0,n=0,j=0;
    if(th!= NULL) {
     i=0; n=0;
     while(th[i]!=NULL) th[i++]->sw = 1;
    }
    return 1;
}
static int ClearThumbNails(ThumbNail **th) {
    int i=0,n=0,j=0;
    if(th!= NULL) {
     i=0; n=0;
     while(th[i]!=NULL) th[i++]->sw = 0;
    }
    return 1;
}

  static int chkfolderstat ( char *folder ) {
      FILE *fp;
      int junk;
      int ret = -1;
      char job [ 500 ] ;
      sprintf ( job , "stat -t \"%s\"" , folder ) ;
      fp = popen ( job , "r" ) ;
      if ( fp != NULL ) {
          fscanf ( fp , "%s%d%d%s%d%d%d%d%d%d%d%d%d" , job , & junk , & junk , job , & junk , & junk , & junk , & junk , & junk , & junk , & junk , & junk , & ret ) ;
              
      }
//	printf("ret = %d\n",ret);
      return ret;
  }
  static char *MakeFileToken ( char *src , char *check , char *token ) {
      char *ret = NULL;
      char *pt;
      if ( src == NULL ) return NULL;
      pt = strstr ( src , check ) ;
      if ( pt == NULL ) return NULL;
      ret = ( char * ) malloc ( strlen ( token ) +1 ) ;
      strcpy ( ret , token ) ;
      return ret;
  }
  static char *kgCheckFileType ( char *name ) {
      FILE *pp;
      char wrk [ 500 ] , tmp [ 500 ] ;
      char *pt;
      char *ret = NULL;
      sprintf ( wrk , "file \'%s\'" , name ) ;
//	printf("%s\n",wrk);
      pp = popen ( wrk , "r" ) ;
      if ( pp == NULL ) return NULL;
      while ( fgets ( tmp , 499 , pp ) != NULL ) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
          pt = strstr ( tmp , "cannot open" ) ;
          if ( pt != NULL ) {pclose ( pp ) ;return NULL;}
          ret = MakeFileToken ( tmp , "executable" , "Exe" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " image data" , "Image" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " MPEG" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " MP4" , "Video" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " WAVE" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " directory" , "Folder" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , "gzip compressed" , "Gzip" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , "tar archive" , "Tar" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , "C source" , "Csource" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , "symbolic link " , "Link" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
//		ret = MakeFileToken(tmp," SVG ","Image");
//		if (ret!= NULL) {pclose(pp);return ret;}
          ret = malloc ( strlen ( "Unknown" ) +1 ) ;
          strcpy ( ret , "Unknown" ) ;
          pclose ( pp ) ;
          return ret;
      }
  }
static   char *kgCheckImageFile ( char *name ) {
      FILE *pp;
      char wrk [ 500 ] , tmp [ 500 ] ;
      char *pt;
      char *ret = NULL;
      sprintf ( wrk , "file \'%s\'" , name ) ;
//	printf("%s\n",wrk);
      pp = popen ( wrk , "r" ) ;
      if ( pp == NULL ) return NULL;
      while ( fgets ( tmp , 499 , pp ) != NULL ) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
          pt = strstr ( tmp , "cannot open" ) ;
          if ( pt != NULL ) {pclose ( pp ) ;return NULL;}
          ret = MakeFileToken ( tmp , " image data" , "Image" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          pclose ( pp ) ;
          return ret;
      }
  }
static   char *kgCheckMediaFile ( char *name ) {
      FILE *pp;
      char wrk [ 500 ] , tmp [ 500 ] ;
      char *pt;
      char *ret = NULL;
      sprintf ( wrk , "file \'%s\'" , name ) ;
//	printf("%s\n",wrk);
      pp = popen ( wrk , "r" ) ;
      if ( pp == NULL ) return NULL;
      while ( fgets ( tmp , 499 , pp ) != NULL ) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
          pt = strstr ( tmp , "cannot open" ) ;
          if ( pt != NULL ) {pclose ( pp ) ;return NULL;}
          ret = MakeFileToken ( tmp , " MPEG" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " MP4" , "Video" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " WAVE" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          pclose ( pp ) ;
          return ret;
      }
  }
static   char *kgCheckVideoFile ( char *name ) {
      FILE *pp;
      char wrk [ 500 ] , tmp [ 500 ] ;
      char *pt;
      char *ret = NULL;
//      printf("Indide CheckMedia : FileType= %d\n",FileType);
      sprintf ( wrk , "file \'%s\'" , name ) ;
//	printf("%s\n",wrk);
      pp = popen ( wrk , "r" ) ;
      if ( pp == NULL ) return NULL;
      while ( fgets ( tmp , 499 , pp ) != NULL ) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
          pt = strstr ( tmp , "cannot open" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " MP4" , "Video" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          pclose ( pp ) ;
          return ret;
      }
  }
static   char *kgCheckAudioFile ( char *name ) {
      FILE *pp;
      char wrk [ 500 ] , tmp [ 500 ] ;
      char *pt;
      char *ret = NULL;
      sprintf ( wrk , "file \'%s\'" , name ) ;
//	printf("%s\n",wrk);
      pp = popen ( wrk , "r" ) ;
      if ( pp == NULL ) return NULL;
      while ( fgets ( tmp , 499 , pp ) != NULL ) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
          pt = strstr ( tmp , "cannot open" ) ;
          if ( pt != NULL ) {pclose ( pp ) ;return NULL;}
          ret = MakeFileToken ( tmp , " MPEG" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " WAVE" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " stereo" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          ret = MakeFileToken ( tmp , " Audio" , "Music" ) ;
          if ( ret != NULL ) {pclose ( pp ) ;return ret;}
          pclose ( pp ) ;
          return ret;
      }
  }
  static int DeleteSelection ( void *Wid ) {
      int i;
      i = 0;
      while ( kgGetThumbNail ( Wid , i ) != NULL ) {
          if ( kgGetSwitch ( Wid , i ) == 1 ) {
              kgDeleteThumbNail ( Wid , i ) ;
          }
          else i++;
      }
      return 1;
  }
  static int ClearSelection ( void *Wid ) {
      int i;
      ThumbNail **Th = ( ThumbNail ** ) kgGetList ( Wid ) ;
      i = 0;
      while ( Th [ i ] != NULL ) {
          Th [ i ] -> sw = 0;
          i++;
      }
      return 1;
  }
  static int SelectAll ( void *Wid ) {
      int i;
      ThumbNail **Th = ( ThumbNail ** ) kgGetList ( Wid ) ;
      i = 0;
      while ( Th [ i ] != NULL ) {
          Th [ i ] -> sw = 1;
          i++;
      }
      return 1;
  }
  static int DragItem ( void *Tmp , void *fw , int item ) {
      DIY *FY , *TY;
      FY = ( DIY * ) fw;
      int x = -1 , y = -1;
      void *tw = NULL;
      int sid , did;
      char *des = NULL;
      int same = 0;
      if ( kgDragThumbNail ( FY , item , & x , & y ) ) {
          tw = ( DIT * ) kgGetLocationWidget ( Tmp , x , y ) ;
          if ( tw != NULL ) {
              did = kgGetWidgetId ( Tmp , tw ) ;
              des = kgGetWidgetName ( Tmp , did ) ;
              if ( strcmp ( des , "Selections" ) == 0 ) {
                  ThumbNail *Th = NULL;
                  char *name;
		  int positem = 1000000;
                  Th = kgCopyThumbNail ( kgGetThumbNail ( fw , item ) ) ;
                  name = ( char * ) malloc ( strlen ( Folder ) +strlen ( Th-> name ) +4 ) ;
                  strcpy ( name , Folder ) ;
                  strcat ( name , "/" ) ;
                  strcat ( name , Th-> name ) ;
                  free ( Th-> name ) ;
                  Th-> name = name; 
		  positem = kgGetThumbNailItem(tw,x,y);
                  kgInsertThumbNail ( tw , Th , positem ) ;
//                  kgSortList ( tw ) ;
                  kgListRemoveDup ( tw ) ;
                  ClearSelection ( tw ) ;
                  kgUpdateWidget ( tw ) ;
                  kgUpdateOn ( Tmp ) ;
                  return 1;
              }
              else return 0;
          }
      }
      else return 0;
  }
  static int Drag2Item ( void *Tmp , void *fw , int item ) {
      DIY *FY , *TY;
      FY = ( DIY * ) fw;
      int x = -1 , y = -1;
      void *tw = NULL;
      int sid , did;
      char *des = NULL;
      int same = 0;
      if ( kgDragThumbNail ( FY , item , & x , & y ) ) {
          tw = ( DIT * ) kgGetLocationWidget ( Tmp , x , y ) ;
          if ( tw != NULL ) {
              did = kgGetWidgetId ( Tmp , tw ) ;
              des = kgGetWidgetName ( Tmp , did ) ;
              if ( strcmp ( des , "Selections" ) == 0 ) {
                  ThumbNail *Th = NULL;
                  char *name;
		  int positem = 1000000;
                  Th = kgCopyThumbNail ( kgGetThumbNail ( fw , item ) ) ;
		  positem = kgGetThumbNailItem(tw,x,y);
                  kgInsertThumbNail ( tw , Th , positem ) ;
//                  kgSortList ( tw ) ;
                  kgListRemoveDup ( tw ) ;
                  ClearSelection ( tw ) ;
                  kgUpdateWidget ( tw ) ;
                  kgUpdateOn ( Tmp ) ;
                  return 1;
              }
              else return 0;
          }
      }
      else return 0;
  }
  static ThumbNail **GetFolderThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFolderThumbNails ( Folder ) ;
      if ( th == NULL ) return NULL;
      if ( folderimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & folder_str ) ;
          if ( imgo != NULL ) {
              folderimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckFileType ( fullname ) ;
          switch ( fret [ 0 ] ) {
              case 'L':
              if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                  ( NULL , ( void * ) & link_str ) ;
              th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
              break;
              default:
              th [ i ] -> img = kgCopyImage ( folderimg ) ;
              break;
          }
          i++;
      }
      return th;
  }
  static ThumbNail **GetFileThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFileThumbNails ( Folder , "*" ) ;
      if ( th == NULL ) return NULL;
      if ( fileimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & file_str ) ;
          if ( imgo != NULL ) {
              fileimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckFileType ( fullname ) ;
//          fret = ( char * ) ((TypeFun[FileType]) ( fullname )) ;
          if ( fret != NULL ) {
              switch ( fret [ 0 ] ) {
                  case 'I':
                  imgo = kgGetImage ( fullname ) ;
                  if ( imgo != NULL ) {
                      th [ i ] -> img = kgThumbNailImage ( imgo , size , size ) ;
                      kgFreeImage ( imgo ) ;
                  }
                  else th [ i ] -> img = kgCopyImage ( fileimg ) ;
                  break;
                  case 'L':
                  if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                      ( NULL , ( void * ) & link_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
                  break;
                  case 'M':
                  if ( musicimg == NULL ) musicimg = kgGetImageCopy 
                      ( NULL , ( void * ) & music_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( musicimg , size , size ) ;
                  break;
                  case 'V':
                  if ( videoimg == NULL ) videoimg = kgGetImageCopy 
                      ( NULL , ( void * ) & video_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( videoimg , size , size ) ;
                  break;
                  case 'O':
                  if ( binaryimg == NULL ) binaryimg = kgGetImageCopy 
                      ( NULL , ( void * ) & binary_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( binaryimg , size , size ) ;
                  break;
                  case 'E':
                  if ( exeimg == NULL ) exeimg = kgGetImageCopy 
                      ( NULL , ( void * ) & exec_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( exeimg , size , size ) ;
                  break;
                  case 'G':
                  if ( gzipimg == NULL ) gzipimg = kgGetImageCopy 
                      ( NULL , ( void * ) & gzip_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( gzipimg , size , size ) ;
                  break;
                  case 'T':
                  if ( tarimg == NULL ) tarimg = kgGetImageCopy 
                      ( NULL , ( void * ) & tar2_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( tarimg , size , size ) ;
                  break;
                  case 'C':
                  if ( cimg == NULL ) cimg = kgGetImageCopy 
                      ( NULL , ( void * ) & source_c_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( cimg , size , size ) ;
                  break;
                  default:
                  th [ i ] -> img = kgCopyImage ( fileimg ) ;
                  break;
              }
          }
          else th [ i ] -> img = kgCopyImage ( fileimg ) ;
          i++;
      }
      return th;
  }
  static ThumbNail **GetMediaFileThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      Dlink *Tlist=Dopen();
      void *tmp;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFileThumbNails ( Folder , "*" ) ;
      if ( th == NULL ) return NULL;
      if ( fileimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & file_str ) ;
          if ( imgo != NULL ) {
              fileimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckMediaFile ( fullname ) ;
          if ( fret != NULL ) {
              switch ( fret [ 0 ] ) {
                  case 'I':
                  imgo = kgGetImage ( fullname ) ;
                  if ( imgo != NULL ) {
                      th [ i ] -> img = kgThumbNailImage ( imgo , size , size ) ;
                      kgFreeImage ( imgo ) ;
                  }
                  else th [ i ] -> img = kgCopyImage ( fileimg ) ;
                  break;
                  case 'L':
                  if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                      ( NULL , ( void * ) & link_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
                  break;
                  case 'M':
                  if ( musicimg == NULL ) musicimg = kgGetImageCopy 
                      ( NULL , ( void * ) & music_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( musicimg , size , size ) ;
		  Dadd(Tlist,th[i]);
                  break;
                  case 'V':
                  if ( videoimg == NULL ) videoimg = kgGetImageCopy 
                      ( NULL , ( void * ) & video_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( videoimg , size , size ) ;
		  Dadd(Tlist,th[i]);
                  break;
                  case 'O':
                  if ( binaryimg == NULL ) binaryimg = kgGetImageCopy 
                      ( NULL , ( void * ) & binary_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( binaryimg , size , size ) ;
                  break;
                  case 'E':
                  if ( exeimg == NULL ) exeimg = kgGetImageCopy 
                      ( NULL , ( void * ) & exec_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( exeimg , size , size ) ;
                  break;
                  case 'G':
                  if ( gzipimg == NULL ) gzipimg = kgGetImageCopy 
                      ( NULL , ( void * ) & gzip_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( gzipimg , size , size ) ;
                  break;
                  case 'T':
                  if ( tarimg == NULL ) tarimg = kgGetImageCopy 
                      ( NULL , ( void * ) & tar2_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( tarimg , size , size ) ;
                  break;
                  case 'C':
                  if ( cimg == NULL ) cimg = kgGetImageCopy 
                      ( NULL , ( void * ) & source_c_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( cimg , size , size ) ;
                  break;
                  default:
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
                  break;
              }
          }
          else {
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
	  }
          i++;
      }
      i=0;
      Resetlink(Tlist);
      while( (tmp=Getrecord(Tlist)) != NULL) th[i++]=(ThumbNail *)tmp;
      th[i]=NULL;
      return th;
  }
  static ThumbNail **GetAudioFileThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      Dlink *Tlist=Dopen();
      void *tmp;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFileThumbNails ( Folder , "*" ) ;
      if ( th == NULL ) return NULL;
      if ( fileimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & file_str ) ;
          if ( imgo != NULL ) {
              fileimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckAudioFile ( fullname ) ;
          if ( fret != NULL ) {
              switch ( fret [ 0 ] ) {
                  case 'I':
                  imgo = kgGetImage ( fullname ) ;
                  if ( imgo != NULL ) {
                      th [ i ] -> img = kgThumbNailImage ( imgo , size , size ) ;
                      kgFreeImage ( imgo ) ;
                  }
                  else th [ i ] -> img = kgCopyImage ( fileimg ) ;
                  break;
                  case 'L':
                  if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                      ( NULL , ( void * ) & link_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
                  break;
                  case 'M':
                  if ( musicimg == NULL ) musicimg = kgGetImageCopy 
                      ( NULL , ( void * ) & music_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( musicimg , size , size ) ;
		  Dadd(Tlist,th[i]);
                  break;
                  case 'V':
                  if ( videoimg == NULL ) videoimg = kgGetImageCopy 
                      ( NULL , ( void * ) & video_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( videoimg , size , size ) ;
                  break;
                  case 'O':
                  if ( binaryimg == NULL ) binaryimg = kgGetImageCopy 
                      ( NULL , ( void * ) & binary_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( binaryimg , size , size ) ;
                  break;
                  case 'E':
                  if ( exeimg == NULL ) exeimg = kgGetImageCopy 
                      ( NULL , ( void * ) & exec_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( exeimg , size , size ) ;
                  break;
                  case 'G':
                  if ( gzipimg == NULL ) gzipimg = kgGetImageCopy 
                      ( NULL , ( void * ) & gzip_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( gzipimg , size , size ) ;
                  break;
                  case 'T':
                  if ( tarimg == NULL ) tarimg = kgGetImageCopy 
                      ( NULL , ( void * ) & tar2_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( tarimg , size , size ) ;
                  break;
                  case 'C':
                  if ( cimg == NULL ) cimg = kgGetImageCopy 
                      ( NULL , ( void * ) & source_c_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( cimg , size , size ) ;
                  break;
                  default:
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
                  break;
              }
          }
          else {
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
	  }
          i++;
      }
      i=0;
      Resetlink(Tlist);
      while( (tmp=Getrecord(Tlist)) != NULL) th[i++]=(ThumbNail *)tmp;
      th[i]=NULL;
      return th;
  }
  static ThumbNail **GetVideoFileThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      Dlink *Tlist=Dopen();
      void *tmp;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFileThumbNails ( Folder , "*" ) ;
      if ( th == NULL ) return NULL;
      if ( fileimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & file_str ) ;
          if ( imgo != NULL ) {
              fileimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckVideoFile ( fullname ) ;
          if ( fret != NULL ) {
              switch ( fret [ 0 ] ) {
                  case 'I':
                  imgo = kgGetImage ( fullname ) ;
                  if ( imgo != NULL ) {
                      th [ i ] -> img = kgThumbNailImage ( imgo , size , size ) ;
                      kgFreeImage ( imgo ) ;
                  }
                  else th [ i ] -> img = kgCopyImage ( fileimg ) ;
                  break;
                  case 'L':
                  if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                      ( NULL , ( void * ) & link_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
                  break;
                  case 'M':
                  if ( musicimg == NULL ) musicimg = kgGetImageCopy 
                      ( NULL , ( void * ) & music_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( musicimg , size , size ) ;
                  break;
                  case 'V':
                  if ( videoimg == NULL ) videoimg = kgGetImageCopy 
                      ( NULL , ( void * ) & video_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( videoimg , size , size ) ;
		  Dadd(Tlist,th[i]);
                  break;
                  case 'O':
                  if ( binaryimg == NULL ) binaryimg = kgGetImageCopy 
                      ( NULL , ( void * ) & binary_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( binaryimg , size , size ) ;
                  break;
                  case 'E':
                  if ( exeimg == NULL ) exeimg = kgGetImageCopy 
                      ( NULL , ( void * ) & exec_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( exeimg , size , size ) ;
                  break;
                  case 'G':
                  if ( gzipimg == NULL ) gzipimg = kgGetImageCopy 
                      ( NULL , ( void * ) & gzip_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( gzipimg , size , size ) ;
                  break;
                  case 'T':
                  if ( tarimg == NULL ) tarimg = kgGetImageCopy 
                      ( NULL , ( void * ) & tar2_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( tarimg , size , size ) ;
                  break;
                  case 'C':
                  if ( cimg == NULL ) cimg = kgGetImageCopy 
                      ( NULL , ( void * ) & source_c_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( cimg , size , size ) ;
                  break;
                  default:
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
                  break;
              }
          }
          else {
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
	  }
          i++;
      }
      i=0;
      Resetlink(Tlist);
      while( (tmp=Getrecord(Tlist)) != NULL) th[i++]=(ThumbNail *)tmp;
      th[i]=NULL;
      return th;
  }
  static ThumbNail **GetImageFileThumbNails ( char *Folder , int size ) {
      ThumbNail **th = NULL;
      Dlink *Tlist=Dopen();
      void *tmp;
      char fullname [ 300 ] ;
      char *fret;
      int i;
      void *imgo = NULL , *img = NULL;
      th = ( ThumbNail ** ) kgFileThumbNails ( Folder , "*" ) ;
      if ( th == NULL ) return NULL;
      if ( fileimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & file_str ) ;
          if ( imgo != NULL ) {
              fileimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      if ( linkimg == NULL ) {
          imgo = ( void * ) kgGetImageCopy ( NULL , ( void * ) & link_str ) ;
          if ( imgo != NULL ) {
              linkimg = kgThumbNailImage ( imgo , size , size ) ;
              kgFreeImage ( imgo ) ;
          }
      }
      i = 0;
      while ( th [ i ] != NULL ) {
          sprintf ( fullname , "%-s/%-s" , Folder , th [ i ] -> name ) ;
          fret = ( char * ) kgCheckImageFile ( fullname ) ;
          if ( fret != NULL ) {
              switch ( fret [ 0 ] ) {
                  case 'I':
                  imgo = kgGetImage ( fullname ) ;
                  if ( imgo != NULL ) {
                      th [ i ] -> img = kgThumbNailImage ( imgo , size , size ) ;
                      kgFreeImage ( imgo ) ;
                  }
                  else th [ i ] -> img = kgCopyImage ( fileimg ) ;
		  Dadd(Tlist,th[i]);
                  break;
                  case 'L':
                  if ( linkimg == NULL ) linkimg = kgGetImageCopy 
                      ( NULL , ( void * ) & link_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( linkimg , size , size ) ;
                  break;
                  case 'M':
                  if ( musicimg == NULL ) musicimg = kgGetImageCopy 
                      ( NULL , ( void * ) & music_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( musicimg , size , size ) ;
                  break;
                  case 'V':
                  if ( videoimg == NULL ) videoimg = kgGetImageCopy 
                      ( NULL , ( void * ) & video_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( videoimg , size , size ) ;
                  break;
                  case 'O':
                  if ( binaryimg == NULL ) binaryimg = kgGetImageCopy 
                      ( NULL , ( void * ) & binary_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( binaryimg , size , size ) ;
                  break;
                  case 'E':
                  if ( exeimg == NULL ) exeimg = kgGetImageCopy 
                      ( NULL , ( void * ) & exec_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( exeimg , size , size ) ;
                  break;
                  case 'G':
                  if ( gzipimg == NULL ) gzipimg = kgGetImageCopy 
                      ( NULL , ( void * ) & gzip_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( gzipimg , size , size ) ;
                  break;
                  case 'T':
                  if ( tarimg == NULL ) tarimg = kgGetImageCopy 
                      ( NULL , ( void * ) & tar2_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( tarimg , size , size ) ;
                  break;
                  case 'C':
                  if ( cimg == NULL ) cimg = kgGetImageCopy 
                      ( NULL , ( void * ) & source_c_str ) ;
                  th [ i ] -> img = kgThumbNailImage ( cimg , size , size ) ;
                  break;
                  default:
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
                  break;
              }
          }
          else {
                  kgFreeThumbNail(th[i]);
		  th[i]=NULL;
	  }
          i++;
      }
      i=0;
      Resetlink(Tlist);
      while( (tmp=Getrecord(Tlist)) != NULL) th[i++]=(ThumbNail *)tmp;
      th[i]=NULL;
      return th;
  }
  int FileListBrowserbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      switch ( butno ) {
          case 1:
          strcpy ( Folder , Home ) ;
          Update1 ( Tmp ) ;
          break;
          case 2:
          if ( strcmp ( Folder , "/" ) != 0 ) {
              char *pt , *pt1 = NULL;
              pt = Folder+1;
              while ( ( pt = strstr ( pt , "/" ) ) != NULL ) {pt1 = pt; pt++;}
              if ( pt1 != NULL ) *pt1 = '\0';
              else strcpy ( Folder , "/" ) ;
              Update1 ( Tmp ) ;
          }
          break;
      }
      return ret;
  }
  void FileListBrowserbutton1init ( DIN *B , void *pt ) {
	  BUT_STR *butns = (BUT_STR *)(B->buts);
	  butns[0].xpmn=kgHomeImage(22,RED,GREEN,BLUE);
	  butns[1].xpmn=kgUpdirImage(22,RED,GREEN,BLUE);
  }
  int FileListBrowsertextbox1callback ( int cellno , int i , void *Tmp ) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
      DIALOG *D;DIT *T;T_ELMT *e;
      int ret = 1;
      char *folder;
      char *fret = NULL;
      D = ( DIALOG * ) Tmp;
      T = ( DIT * ) kgGetWidget ( Tmp , i ) ;
      e = T-> elmt;
      folder = ( char * ) kgGetString ( T , cellno ) ;
      fret = kgCheckFileType ( folder ) ;
      if ( ( fret == NULL ) || ( fret [ 0 ] != 'F' ) ) {
          kgSetString ( T , cellno , Folder ) ;
          kgUpdateWidget ( T ) ;
          kgUpdateOn ( Tmp ) ;
      }
      else {
          strcpy ( Folder , folder ) ;
          Update1 ( Tmp ) ;
      }
      kgFree ( fret ) ;
  }
  static void Update1 ( void *Tmp ) {
      void **th;
      void *bs = NULL;
      bs = kgOpenBusy ( Tmp , 600 , 250 ) ;
      kgSetString ( T1 , 0 , Folder ) ;
      th = ( void ** ) kgGetList ( X1 ) ;
      if ( th != NULL ) kgFreeThumbNails ( ( ThumbNail ** ) th ) ;
      th = ( void ** ) GetFolderThumbNails ( Folder , 20 ) ;
      kgSetList ( X1 , th ) ;
      th = ( void ** ) kgGetList ( Y1 ) ;
      if ( th != NULL ) kgFreeThumbNails ( ( ThumbNail ** ) th ) ;
#if 1
      switch(FileType) {
	      case 1:
              th = ( void ** ) GetMediaFileThumbNails ( Folder , 48 ) ;
	      break;
	      case 2:
              th = ( void ** ) GetAudioFileThumbNails ( Folder , 48 ) ;
	      break;
	      case 3:
              th = ( void ** ) GetVideoFileThumbNails ( Folder , 48 ) ;
	      break;
	      case 4:
              th = ( void ** ) GetImageFileThumbNails ( Folder , 48 ) ;
	      break;
	      default:
	      th = ( void ** ) GetFileThumbNails ( Folder , 48 ) ;
	      break;
      }
#else
      th = ( void ** ) GetFileThumbNails ( Folder , 48 ) ;
#endif
      kgCloseBusy ( bs ) ;
      kgSetList ( Y1 , th ) ;
      kgUpdateWidget ( X1 ) ;
      kgUpdateWidget ( Y1 ) ;
      kgUpdateWidget ( T1 ) ;
      kgUpdateOn ( Tmp ) ;
      folder1stat = chkfolderstat ( Folder ) ;
  }
  int FileListBrowserbrowser1callback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIX *X;void *pt;
      int ret = 1;
      ThumbNail *th;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      X = ( DIX * ) kgGetWidget ( Tmp , i ) ;
      th = ( ThumbNail * ) kgPickThumbNail ( X , item-1 ) ;
      if ( strcmp ( Folder , "/" ) != 0 ) strcat ( Folder , "/" ) ;
      strcat ( Folder , th-> name ) ;
      Update1 ( Tmp ) ;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  void FileListBrowserbrowser1init ( DIX *X , void *pt ) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
      void **th;
      strcpy ( Home , getenv ( "HOME" ) ) ;
      th = ( void ** ) GetFolderThumbNails ( Home , 20 ) ;
      kgSetList ( X , th ) ;
  }
  int FileListBrowserbrowser2callback ( int item , int i , void *Tmp ) {
      DIALOG *D;DIY *Y;void *pt;
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      Y = ( DIY * ) kgGetWidget ( Tmp , i ) ;
      if ( DragItem ( Tmp , Y1 , item-1 ) ) return 1;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  void FileListBrowserbrowser2init ( DIY *Y , void *pt ) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
      void **th;
      strcpy ( Home , getenv ( "HOME" ) ) ;
#if 0
#ifdef D_ALLFILES
      th = ( void ** ) GetFileThumbNails ( Home , 20 ) ;
#else
      th = ( void ** ) GetMediaFileThumbNails ( Home , 20 ) ;
#endif
#endif
#if 1
      switch(FileType) {
	      case 1:
              th = ( void ** ) GetMediaFileThumbNails ( Home , 48 ) ;
	      break;
	      case 2:
              th = ( void ** ) GetAudioFileThumbNails ( Home , 48 ) ;
	      break;
	      case 3:
              th = ( void ** ) GetVideoFileThumbNails ( Home , 48 ) ;
	      break;
	      case 4:
              th = ( void ** ) GetImageFileThumbNails ( Home , 48 ) ;
	      break;
	      default:
	      th = ( void ** ) GetFileThumbNails ( Home , 48 ) ;
	      break;
      }
#else
      th = ( void ** ) GetFileThumbNails ( Folder , 48 ) ;
#endif

      kgSetList ( Y , th ) ;
  }
  int FileListBrowserbrowser3callback ( int item , int i , void *Tmp ) {
      DIALOG *D;DIY *Y;void *pt;
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      Y = ( DIY * ) kgGetWidget ( Tmp , i ) ;
      if ( Drag2Item ( Tmp , Y2 , item-1 ) ) return 1;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  void FileListBrowserbrowser3init ( DIY *Y , void *pt ) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
  }
  int FileListBrowserbutton2callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      ThumbNail **th;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      switch ( butno ) {
          case 1:
	  th = (ThumbNail **) kgGetList(Y2);
          ClearThumbNails(th);
          kgSetList(Y2,(void **)th);
          kgUpdateWidget(Y2);
          kgUpdateOn(D);
          break;
          case 2:
	  th = (ThumbNail **) kgGetList(Y2);
          SetThumbNails(th);
          kgSetList(Y2,(void **)th);
          kgUpdateWidget(Y2);
          kgUpdateOn(D);
          break;
          case 3:
          DeleteSelection ( Y2 ) ;
          kgUpdateWidget ( Y2 ) ;
          kgUpdateOn ( Tmp ) ;
          break;
      }
      return ret;
  }
  void FileListBrowserbutton2init ( DIN *B , void *pt ) {
  }
  int FileListBrowsersplbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      int n , ret = 1 , j = 0;;
      ThumbNail **th;
      char **Strs = NULL;
      char ***pt;
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx;
      th = ( ThumbNail ** ) kgGetList ( Y2 ) ;
      if(th == NULL) return ret;
      j = 0;
      while ( th [ j ] != NULL ) j++;
      if(j == 0) return ret;
      if ( j > 0 ) {
          Strs = ( char ** ) malloc ( sizeof ( char * ) * ( j+1 ) ) ;
      }
      n = j;
      for ( j = 0;j < n;j++ ) {
          Strs [ j ] = ( char * ) malloc ( strlen ( th [ j ] -> name ) +1 ) ;
          strcpy ( Strs [ j ] , th [ j ] -> name ) ;
      }
      Strs [ n ] = NULL;
      pt = ( char *** ) D-> pt;
      *pt = Strs;
      switch ( butno ) {
          case 1:
          break;
      }
      return ret;
  }
  void FileListBrowsersplbutton1init ( DIL *B , void *pt ) {
  }
  static int CopyItems ( void *Tmp , void *fw ) {
      DIY *FY , *TY;
      int item = 0 , i = 0 , j = 0;
      int Added=0;
      ThumbNail **th;
      FY = ( DIY * ) fw;
      int x = -1 , y = -1;
      void *tw = NULL;
      j = 0;
      th = ( ThumbNail ** ) kgGetList ( Y1 ) ;
      while ( th [ j ] != NULL ) {
          if ( kgGetSwitch ( Y1 , j ) == 1 ) {
		  Added =1;
	  }
	  j++;
      }
      if(Added==0) return 0;
/* Warning supressed */
//      if ( ! kgCheckMenu ( Tmp , 200 , 250 , "Add Selected" , 0 ) ) return 0;
//      th = ( ThumbNail ** ) kgGetList ( Y1 ) ;
      j = 0;
      while ( th [ j ] != NULL ) {
          if ( kgGetSwitch ( Y1 , j ) == 1 ) {
              ThumbNail *TH = NULL;
              char *name;
//              TH = kgCopyThumbNail ( kgGetThumbNail ( FY , j ) ) ;
              TH = kgCopyThumbNail ( th[j] ) ;
              name = ( char * ) malloc ( strlen ( Folder ) +strlen ( TH-> name ) +4 ) ;
              strcpy ( name , Folder ) ;
              strcat ( name , "/" ) ;
              strcat ( name , TH-> name ) ;
              free ( TH-> name ) ;
              TH-> name = name;
              kgAddThumbNail ( Y2 , TH , 1000000 ) ;
	      Added=1;
          }
          j++;
      }
//      kgSortList ( Y2 ) ;
      kgListRemoveDup ( Y2 ) ;
      ClearSelection ( Y2 ) ;
      kgUpdateWidget ( Y2 ) ;
      kgUpdateOn ( Tmp ) ;
      return 1;
  }
  int FileListBrowserbutton3callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0 , j = 0;
      ThumbNail **th = NULL;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      switch ( butno ) {
          case 1:
          th = ( ThumbNail ** ) kgGetList ( Y1 ) ;
          j = 0;
          while ( th [ j ] != NULL ) {
              kgSetSwitch ( Y1 , j , 0 ) ;
              j++;
          };
          kgUpdateWidget ( Y1 ) ;
          kgUpdateOn ( Tmp ) ;
          break;
          case 2:
//      SelectAll(Y1);
          th = ( ThumbNail ** ) kgGetList ( Y1 ) ;
          j = 0;
          while ( th [ j ] != NULL ) {
              kgSetSwitch ( Y1 , j , 1 ) ;
              j++;
          };
          kgUpdateWidget ( Y1 ) ;
          kgUpdateOn ( Tmp ) ;
          break;
          case 3:
          CopyItems ( Tmp , Y1 ) ;
          break;
      }
      return ret;
  }
  void FileListBrowserbutton3init ( DIN *B , void *pt ) {
  }
  int FileListBrowserinit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      T1 = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "TextBox" ) ;
      X1 = ( DIX * ) kgGetNamedWidget ( Tmp , ( char * ) "Folders" ) ;
      Y1 = ( DIY * ) kgGetNamedWidget ( Tmp , ( char * ) "FileList" ) ;
      Y2 = ( DIY * ) kgGetNamedWidget ( Tmp , ( char * ) "Selections" ) ;
      if ( Home [ 0 ] == '\0' ) strcpy ( Home , getenv ( "HOME" ) ) ;
      strcpy ( Folder , Home ) ;
      kgSetString ( T1 , 0 , Home ) ;
      kgUpdateWidget ( T1 ) ;
      return ret;
  }
  int FileListBrowsercleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      return ret;
  }
  int ModifyFileListBrowser ( void *Tmp , int GrpId ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIA *d;
      int i , n;
      d = D-> d;
      i = 0;while ( d [ i ] .t != NULL ) {;
          i++;
      };
      n = 1;
      return GrpId;
  }
  int FileListBrowserCallBack ( void *Tmp , void *tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe-> event == 1 ) {
          if ( kbe-> button == 1 ) {
          }
      }
      return ret;
  }
  int FileListBrowserResizeCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 0;
      int xres , yres , dx , dy;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      kgGetWindowSize ( D , & xres , & yres ) ;
      dx = xres - D-> xl;
      dy = yres - D-> yl;
  /* extra code */
      D-> xl = xres;
      D-> yl = yres;
      kgRedrawDialog ( D ) ;
      return ret;
  }
  int FileListBrowserWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      return ret;
  }
