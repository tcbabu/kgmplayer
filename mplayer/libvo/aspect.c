/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#define D_KULINA
/* Stuff for correct aspect scaling. */
#include "aspect.h"
#include "geometry.h"
//#ifndef ASPECT_TEST
#include "mp_msg.h"
#include "help_mp.h"
//#endif

//#define ASPECT_DEBUG

#if defined(ASPECT_DEBUG) || defined(ASPECT_TEST)
#include <stdio.h>
#endif
#ifdef D_KULINA
#include <stdio.h>
extern int Kulina;
extern float Xs,Ys;
#endif

int vo_panscan_x = 0;
int vo_panscan_y = 0;
float vo_panscan_amount = 0;
float vo_panscanrange = 1.0;

#include "video_out.h"

float force_monitor_aspect=0;
float monitor_aspect=0;
float monitor_pixel_aspect=1;

static struct {
  int orgw; // real width
  int orgh; // real height
  int prew; // prescaled width
  int preh; // prescaled height
  int orgw_norot;
  int orgh_norot;
  int prew_norot;
  int preh_norot;
  int scrw; // horizontal resolution
  int scrh; // vertical resolution
  float asp;
} aspdat;

static void aspect_rotate(void) {
  if (vo_rotate & 1) {
    aspdat.orgw = aspdat.orgh_norot;
    aspdat.orgh = aspdat.orgw_norot;
    aspdat.prew = aspdat.preh_norot;
    aspdat.preh = aspdat.prew_norot;
  } else {
    aspdat.orgw = aspdat.orgw_norot;
    aspdat.orgh = aspdat.orgh_norot;
    aspdat.prew = aspdat.prew_norot;
    aspdat.preh = aspdat.preh_norot;
  }
}

void aspect_save_orig(int orgw, int orgh){
#ifdef ASPECT_DEBUG
  printf("aspect_save_orig %dx%d \n",orgw,orgh);
#endif
  aspdat.orgw_norot = orgw;
  aspdat.orgh_norot = orgh;
}

void aspect_save_prescale(int prew, int preh){
#ifdef ASPECT_DEBUG
  printf("aspect_save_prescale %dx%d \n",prew,preh);
#endif
  aspdat.prew_norot = prew;
  aspdat.preh_norot = preh;

#ifdef D_KULINA
  if(Kulina) {
#if 1
  aspdat.prew_norot = ((prew*(1+Ys))/2)*2;
  aspdat.preh_norot = ((preh*(1+Ys))/2)*2;
//  fprintf(stderr,"aspect_save_prescale %dx%d \n",prew,preh);
//  fprintf(stderr,"aspect_save_prescale %dx%d \n",aspdat.prew_norot,aspdat.preh_norot);
#endif
  }
#endif
}
void aspect_save_screenres(int scrw, int scrh){
#ifdef ASPECT_DEBUG
  printf("aspect_save_screenres %dx%d \n",scrw,scrh);
#endif
  if (scrw <= 0 && scrh <= 0)
    scrw = 1024;
  if (scrh <= 0)
    scrh = (scrw * 3 + 3) / 4;
  if (scrw <= 0)
    scrw = (scrh * 4 + 2) / 3;
  aspdat.scrw = scrw;
  aspdat.scrh = scrh;
  if (force_monitor_aspect)
    monitor_aspect = force_monitor_aspect;
  else
    monitor_aspect = monitor_pixel_aspect * scrw / scrh;
}

/* aspect is called with the source resolution and the
 * resolution, that the scaled image should fit into
 */

void aspect_fit(int *srcw, int *srch, int fitw, int fith){
  int tmpw;

  aspect_rotate();
#ifdef ASPECT_DEBUG
  printf("aspect(0) fitin: %dx%d screenaspect: %.2f\n",aspdat.scrw,aspdat.scrh,
      monitor_aspect);
  printf("aspect(1) wh: %dx%d (org: %dx%d)\n",*srcw,*srch,aspdat.prew,aspdat.preh);
#endif
    *srcw = fitw;
    *srch = (int)(((float)fitw / (float)aspdat.prew * (float)aspdat.preh)
               * ((float)aspdat.scrh / ((float)aspdat.scrw / monitor_aspect)));
  *srch+= *srch%2; // round
#ifdef ASPECT_DEBUG
  printf("aspect(2) wh: %dx%d (org: %dx%d)\n",*srcw,*srch,aspdat.prew,aspdat.preh);
#endif
  if(*srch>fith || *srch<aspdat.orgh){
      tmpw = (int)(((float)fith / (float)aspdat.preh * (float)aspdat.prew)
                * ((float)aspdat.scrw / ((float)aspdat.scrh / (1.0/monitor_aspect))));
    tmpw+= tmpw%2; // round
    if(tmpw<=fitw /*&& tmpw>=aspdat.orgw*/){
      *srch = fith;
      *srcw = tmpw;
    }else{
#ifndef ASPECT_TEST
      mp_msg(MSGT_VO,MSGL_WARN,MSGTR_LIBVO_ASPECT_NoSuitableNewResFound);
#else
      mp_msg(MSGT_VO,MSGL_WARN,MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes);
#endif
    }
  }
  aspdat.asp=*srcw / (float)*srch;
#ifdef D_KULINA
#if 1
  if(Kulina) {
    if( (Xs>0.01)||(Ys>0.01)) {
#if 1
      *srch= *srch*(1+Ys); // round
      *srcw= *srcw*(1+Ys); // round
      *srch+= *srch%2; // round
      *srcw+= *srcw%2; // round
//    aspdat.asp=*srcw / (float)*srch;
#else
    float fac;
    fac = ((float)aspdat.scrh / ((float)aspdat.scrw / monitor_aspect));
    *srch = fith;
    *srcw = (int)((float)(*srch) / (float)aspdat.preh * (float)aspdat.prew)/fac;
    if( *srcw < fitw/fac) {
      *srcw = fitw/fac;
      *srch = (int)((float)(*srcw) / (float)aspdat.prew * (float)aspdat.preh);
      *srch *=fac;
    }
    *srcw = fitw;
    *srch+= *srch%2; // round
    *srcw+= *srcw%2; // round
    aspdat.asp=*srcw / (float)*srch;
#endif
   }
  }
//      else if(*srch<fith) {*srcw =*srcw*fith/(float)*srch;
//      *srch=fith;}
#endif
#endif
#ifdef ASPECT_DEBUG
  printf("aspect(3) wh: %dx%d (org: %dx%d)\n",*srcw,*srch,aspdat.prew,aspdat.preh);
#endif
}

static void get_max_dims(int *w, int *h, int zoom)
{
  *w = zoom ? aspdat.scrw : aspdat.prew;
  *h = zoom ? aspdat.scrh : aspdat.preh;
  if (zoom && WinID >= 0) zoom = A_WINZOOM;
  if (zoom == A_WINZOOM) {
    *w = vo_dwidth;
    *h = vo_dheight;
  }
}

void aspect(int *srcw, int *srch, int zoom){
  int fitw;
  int fith;
  aspect_rotate();
  get_max_dims(&fitw, &fith, zoom);
  if( !zoom && geometry_wh_changed ) {
#ifdef ASPECT_DEBUG
    printf("aspect(0) no aspect forced!\n");
#endif
    return; // the user doesn't want to fix aspect
  }
  aspect_fit(srcw, srch, fitw, fith);
}

void panscan_init( void )
{
 vo_panscan_x=0;
 vo_panscan_y=0;
 vo_panscan_amount=0.0f;
}

static void panscan_calc_internal(int zoom)
{
 int fwidth,fheight;
 int vo_panscan_area;
 int max_w, max_h;
 aspect_rotate();
 get_max_dims(&max_w, &max_h, zoom);

 if (vo_panscanrange > 0) {
   aspect(&fwidth,&fheight,zoom);
   vo_panscan_area = max_h - fheight;
   if (!vo_panscan_area)
     vo_panscan_area = max_w - fwidth;
   vo_panscan_area *= vo_panscanrange;
 } else
   vo_panscan_area = -vo_panscanrange * max_h;

 vo_panscan_amount = vo_fs || zoom == A_WINZOOM ? vo_panscan : 0;
 vo_panscan_x = vo_panscan_area * vo_panscan_amount * aspdat.asp;
 vo_panscan_y = vo_panscan_area * vo_panscan_amount;
}

void panscan_calc(void)
{
  panscan_calc_internal(A_ZOOM);
}

/**
 * vos that set vo_dwidth and v_dheight correctly should call this to update
 * vo_panscan_x and vo_panscan_y
 */
void panscan_calc_windowed(void)
{
  panscan_calc_internal(A_WINZOOM);
}
