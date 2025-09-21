static int ProcessVolume(int pip0,int pip1,int Pid) {
     char buff[1000],dummy[100];
     int ch,i=0,j;

     ttotal = 0;
     duration =0;
     meanVol = -25;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         if((i=SearchString(buff,(char *)"Duration:"))>=0) {
           int j=i;
           while((buff[j] >= ' ')||(buff[j]=='\t')) {
            if(buff[j]==':') buff[j]=' ';
            if(buff[j]==',') buff[j]=' ';
            j++;
           }
           sscanf(buff+i,"%s%lf%lf%lf", dummy,&hr,&min,&sec);
           duration = hr*3600+min*60+sec;
           duration = duration*1000;
         }
         if((i=SearchString(buff,(char *)"mean_volume:"))>=0) {
           sscanf(buff+i,"%s%lf", dummy,&meanVol);
         }
         if((i=SearchString(buff,(char *)"max_volume:"))>=0) {
           sscanf(buff+i,"%s%lf", dummy,&maxVol);
         }
         if((i=SearchString(buff,(char *)"histogram_"))>=0) {
           int j=0;
           i = i+10;
           j = i;
           while(buff[j] != 'd') j++;
           buff[j]=' ';
           sscanf(buff+i,"%lf", &histVol);
           histVol = -histVol;
           if((j=SearchString(buff+i,(char *)"b:"))>=0) {
             sscanf(buff+i+j+2,"%d",&tcount);
             ttotal +=tcount;
           }
         }
     }
     sprintf(buff,"Mean Volume : %lf Max Vol %lf Hist Vol %lf\n",meanVol,maxVol,histVol);
     vrange = maxVol - meanVol;
     return 1;
}
