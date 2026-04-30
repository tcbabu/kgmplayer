int uiUserImageBox(IMGSTR *IMG,float t_angle,float x1,float y1,\
    float cfx,float cfy,float *XX1,float *YY1,float *XX2,float *YY2){
/*   Gets the Image box with x1,y1 as point
     of rotation, specified in user space.
     Useful only for IMGSTR returned by ftGstring...
     as (0,yl) is mapped to x1,y1
**************************************************/
        int xsize,ysize,xsizeo,ysizeo;
        float X1,Y1,X2,Y2,yl,yu;

        double angle1;
        double rad = 0.017453293;
        yl = -(IMG->yln/cfy); 
        xsizeo = IMG->xln;
        ysizeo = IMG->Size;
        yu = ysizeo/cfy - yl; 
        kgGetImageSize(IMG->img,&xsize,&ysize);
        if( (st >= 0.) && (ct>=0.)){
           angle1 = t_angle;
           X1 = x1-yu*sin(angle1*rad);;
           Y1 = y1-yl*cos(angle1*rad);;
           X2 = (X1+xsize/cfx);
           Y2 = (Y1+ysize/cfy);
        }
        if( (st >= 0.) && (ct< 0.)){
           angle1 = t_angle -90.;
           Y1 = y1-yu*sin(angle1*rad);
           X1 = x1 + yl*cos(angle1*rad);
           X2 = (X1-xsize/cfx);
           Y2 = (Y1+ysize/cfy);
        }
        if( (st < 0.) && (ct<= 0.)){
            angle1 = t_angle -180.;
            Y1 = y1+yl*cos(angle1*rad);;
            X1 = x1+yu*sin(angle1*rd);;
            X2 = (X1-xsize/cfx);
            Y2 = (Y1-ysize/cfy);
        }
        if( (st < 0.) && (ct>0.)){
            angle1 =90.- ( t_angle -270.);
            X1 = x1 - yl*sin(angle1*rad);
            Y1 = y1 + yu*cos(angle1*rad);
            X2 = (X1+xsize/cfx);
            Y2 = (Y1-ysize/cfy);
        }
        *XX1 = X1;
        *YY1 = Y1;
        *XX2 = X2;
        *YY2 = Y2;
        return 1;
}
