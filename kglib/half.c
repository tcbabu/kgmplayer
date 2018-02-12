unsigned long *HalfImage(unsigned long *image,int colums,int rows)
{
#define Minify(weight)                          \
  total.red+=(weight)*(r->red);                 \
  total.green+=(weight)*(r->green);             \
  total.blue+=(weight)*(r->blue);               \
  total.opacity+=(weight)*(r->opacity);         \
  r++;

  unsigned long *minify_image;

  long y;
 
  /*
    Initialize minified image.
  */
  minify_image= image;
  /*
    Reduce each row.
  */
  {
    unsigned long row_count=0,zero=0;
    float red,green,blue,opacity;

    zero=0;
    for (y=0; y < (long) minify_image->rows; y++)
      {
        DoublePixelPacket
          total;

        register const PixelPacket
          *p,
          *r;

        register long
          x;

        register PixelPacket
          *q;

        MagickBool
          thread_status;


        p=AcquireImagePixels(image,-2,2*(y-1),image->columns+4,4,exception);
        q=SetImagePixelsEx(minify_image,0,y,minify_image->columns,1,exception);
        for (x=0; x < columns/2; x++)
              {
                /*
                  Compute weighted average of target pixel color components.
                */
                total=zero;
                r=p;
                Minify(3.0); Minify(7.0);  Minify(7.0);  Minify(3.0);
                r=p+(image->columns+4);
                Minify(7.0); Minify(15.0); Minify(15.0); Minify(7.0);
                r=p+2*(image->columns+4);
                Minify(7.0); Minify(15.0); Minify(15.0); Minify(7.0);
                r=p+3*(image->columns+4);
                Minify(3.0); Minify(7.0);  Minify(7.0);  Minify(3.0);
                q->red=(Quantum) (total.red/128.0+0.5);
                q->green=(Quantum) (total.green/128.0+0.5);
                q->blue=(Quantum) (total.blue/128.0+0.5);
                q->opacity=(Quantum) (total.opacity/128.0+0.5);
                p+=2;
                q++;
              }
            if (!SyncImagePixelsEx(minify_image,exception))
              thread_status=MagickFail;
#if defined(HAVE_OPENMP)
#  pragma omp critical (GM_MinifyImage)
#endif
        {
          row_count++;
          if (QuantumTick(row_count,image->rows))
            if (!MagickMonitorFormatted(row_count,image->rows,exception,
                                        MinifyImageText,image->filename))
              thread_status=MagickFail;
          
          if (thread_status == MagickFail)
            status=MagickFail;
        }
      }
  }
  minify_image->is_grayscale=image->is_grayscale;
  return(minify_image);
}
