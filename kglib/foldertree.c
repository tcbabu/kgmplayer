#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "dlink.h"
#define Malloc(x)  malloc(x)
  char ** _uiFolderMenu ( char *d_name ) {
      DIR *dp;
      char **menu , *item;
      Dlink *L;
      struct dirent *pt = NULL;
      struct stat st;
      int ln , nd = 0 , i , type;
      char buf [ 500 ] ;
      dp = opendir ( d_name ) ;
      if ( dp == NULL ) {
          fprintf (stderr, "No such dir:%s\n" , d_name ) ;
          return NULL;
      }
      L = Dopen ( ) ;
      while ( ( pt = readdir ( dp ) ) != NULL ) {
          if ( pt->d_name [ 0 ] == '.' ) continue;
          strcpy ( buf , d_name ) ;
          strcat ( buf , "/" ) ;
          strcat ( buf , pt->d_name ) ;
          stat ( buf , & st ) ;
          if ( S_ISDIR ( st.st_mode ) ) {
//     if(((st.st_mode & S_IFMT)==S_IFDIR)) {
              ln = strlen ( pt->d_name ) ;
              item = ( char * ) Malloc ( ln+5 ) ;
              strcpy ( item , pt->d_name ) ;
              Dadd ( L , item ) ;
//      printf("%s\n",item);
              nd++;
          }
      }
      closedir ( dp ) ;
      menu = ( char ** ) Malloc ( sizeof ( char * ) * ( nd+1 ) ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( item = ( char * ) Getrecord ( L ) ) != NULL ) {
          menu [ i ] = item;
//          printf("Menu: %s\n",item);
          i++;
      }
      menu [ i ] = NULL;
//      arrange ( menu , i ) ;
      Dfree ( L ) ;
      return menu;
  }
  Dlink *uiFolderTree(char *Folder) {
     char **Fpt;
     void *pt;
     char *dpt;
     char SubFolder[1000];
     int i;
     Dlink *L, *LD;
     Fpt = _uiFolderMenu(Folder);
     if(Fpt == NULL) return NULL;
     if(Fpt[0]== NULL) {free(Fpt);return NULL;}
//     if(Fpt[0]=='\0') {kgFreeDouble(Fpt); return NULL;}
     if(Fpt[0][0]=='\0') {free(Fpt); return NULL;}
     i=0;
     L = Dopen();
     while(Fpt[i]!= NULL) {
        strcpy(SubFolder,Folder); 
        strcat(SubFolder,"/");
        strcat(SubFolder,Fpt[i]);   
        dpt = (char *)malloc(strlen(SubFolder)+1);
        strcpy(dpt,SubFolder);
//        printf("SubFolder: %s\n",SubFolder);
        Dadd(L,dpt);
        LD = uiFolderTree(SubFolder);
        if(LD != NULL) {        
           Resetlink(LD);          
           while ((pt=Getrecord(LD))!= NULL) {
             Dadd(L,pt);
           }
           Dfree(LD);
        }
        i++;
     }
     return L;    
  }
  char **kgFolderTree(char *Folder) {
     char **ret=NULL;
     int i=0;
     char *pt;
     Dlink *L = uiFolderTree(Folder);
     if(L != NULL) {
         ret = (char **)malloc(sizeof(char *)*(Dcount(L)+1));
         Resetlink(L);         
         i=0;
         while( (pt=(char *)Getrecord(L)) != NULL) {
            ret[i++] = pt;
         }
         ret[i]=NULL;
         Dfree(L);
     }
     return ret;
  }
  int main() {
    char **pt= kgFolderTree("/usr/share/fonts");
    if(pt != NULL) {
      int i=0;
      while(pt[i]!= NULL) {
        printf("%s\n",pt[i]);
        i++;
      }
    }
    return 0;
 }
