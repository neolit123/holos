/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_DMC_included
#define h_DMC_included
//----------------------------------------------------------------------
//
// Dynamic Markov Compression (DMC)
//
// http://en.wikipedia.org/wiki/Dynamic_Markov_Compression
// http://plg1.cs.uwaterloo.ca/~ftp/dmc/

#ifndef h_EOF
  #define h_EOF -1
#endif

#define h_dmc_threshold 8
#define h_dmc_bigthresh 8

// threshold = 32;//2;
// bigthresh = 32;//2;

//----------------------------------------------------------------------

struct h_dmc_node
{
  float     count[2];
  h_dmc_node* next[2];
};

//----------------------------------------------------------------------

class h_DMC
{
  private:
    unsigned char*  rbuf;
    int             rbufsize;
    int             rbufpos;
    unsigned char*  wbuf;
  //int             wbufsize;
    int             wbufpos;
  private:
    int       memsize;
    int       threshold;
    int       bigthresh;
    h_dmc_node  *P;
    h_dmc_node  *N;
    h_dmc_node  nodes[256][256];
    h_dmc_node  *nodebuf;
    h_dmc_node  *nodemaxp;
    h_dmc_node  *nodesptr;

  public:

    h_DMC()
      {
        rbuf = NULL;
        rbufsize = 0;
        rbufpos = 0;
        wbuf = NULL;
      //wbufsize = 0;
        wbufpos = 0;
        memsize   = 0x1000000;
        threshold = h_dmc_threshold;//2;
        bigthresh = h_dmc_bigthresh;//2;
        nodebuf = NULL;
      }

    //----------

    ~h_DMC()
      {
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    int get_char(void)
      {
        //return getchar();
        unsigned char c;
        if (rbufpos<rbufsize)
        {
          c = rbuf[rbufpos++];
          return (int)c;
        }
        else return h_EOF;//-1;
      }

    //----------

    void/*int*/ put_char(int c)
      {
        //putchar(c);
        //if (wbufpos<wbufsize)
        //{
          unsigned char ch = (unsigned char)c;
          wbuf[wbufpos++] = ch;
          //return (int)ch;
        //}
        //else return h_EOF;//-1
      }

    //--------------------------------------------------
    //
    // dmc (dynamic markov compression
    //
    //--------------------------------------------------

    void p_init(int memsize)
      {
        //fprintf(stderr,"using %d bytes of predictor memory\n",memsize);
        nodebuf = (h_dmc_node *)h_Malloc(memsize);
        //if (nodebuf == (h_dmc_node *)NULL)
        //{
        //  fprintf(stderr,"memory alloc failed; try smaller predictor memory\n");
        //  exit(1);
        //}
        nodemaxp = nodebuf + (memsize/sizeof(h_dmc_node)) - 20;
        p_flush();
      }

    void p_cleanup(void)
      {
        if (nodebuf) h_Free(nodebuf);
        nodebuf = NULL;
      }

    //----------

    void p_flush(void)
      {
        int i,j;
        for (j=0;j<256;j++)
        {
          for (i=0;i<127;i++)
          {
            nodes[j][i].count[0] = 0.2;
            nodes[j][i].count[1] = 0.2;
            nodes[j][i].next[0] = &nodes[j][2*i + 1];
            nodes[j][i].next[1] = &nodes[j][2*i + 2];
          }
          for (i=127;i<255;i++)
          {
            nodes[j][i].count[0] = 0.2;
            nodes[j][i].count[1] = 0.2;
            nodes[j][i].next[0] = &nodes[i+1][0];
            nodes[j][i].next[1] = &nodes[i-127][0];
          }
        }
        nodesptr = nodebuf;
        p_reset();
      }

    //----------

    void p_reset(void)
      {
        P = &nodes[0][0];
      }

    //----------

    float predict(void)
      {
        return P->count[0] / (P->count[0] + P->count[1]);
      }

    //----------

    void p_update(int b)
      {
        float r;
        if (P->count[b] >= threshold && P->next[b]->count[0]+P->next[b]->count[1] >= bigthresh + P->count[b])
        {
          N = nodesptr++;
          P->next[b]->count[0] -= N->count[0] = P->next[b]->count[0] * (r = P->count[b]/(P->next[b]->count[1]+P->next[b]->count[0]));
          P->next[b]->count[1] -= N->count[1] = P->next[b]->count[1] * r;
          N->next[0] = P->next[b]->next[0];
          N->next[1] = P->next[b]->next[1];
          P->next[b] = N;
        }
        P->count[b]++;
        P = P->next[b];
        if (nodesptr > nodemaxp)
        {
          //fprintf(stderr,"flushing ...\n");
          p_flush();
        }
      }

    //--------------------------------------------------

#ifndef H_NODMCCOMPRESS
    int compress(unsigned char* outbuf, unsigned char* inbuf, int insize)
      {
        rbuf     = inbuf;
        rbufsize = insize;
        rbufpos  = 0;
        wbuf     = outbuf;
      //wbufsize = 0;
        wbufpos  = 0;
        int max = 0x1000000;
        int min = 0;
        int inbytes  = 0;
        int outbytes = 3;
        int pout     = 3;

        p_init(memsize);

        //TODO: for (0..buffersize)
        for(;;)
        {
/**/      int c = get_char();
/**/      if (c == h_EOF)
          {
            min = max-1;
            //fprintf(stderr,"compress done: bytes in %d, bytes out %d, ratio %f\n",inbytes,outbytes,(float)outbytes/inbytes);
            break;
          }

          for (int i=0;i<8;i++)
          {
            int bit = (c << i) & 0x80;
            int mid = min + (max-min-1) * predict();
            p_update(bit != 0);
            if (mid == min) mid++;
            if (mid == (max-1)) mid--;
            if (bit) { min = mid; }
            else { max = mid; }
            while ((max-min) < 256)
            {
              if (bit) max--;
/**/          put_char(min >> 16);
              outbytes++;
              min = (min << 8) & 0xffff00;
              max = ((max << 8) & 0xffff00 ) ;
              if (min >= max) max = 0x1000000;
            } //max min
          } //i

          if (!(++inbytes & 0xff))
          {
            //if (!(inbytes & 0xffff))
            //{
            //  //fprintf(stderr,"compressing... bytes in %d, bytes out %d, ratio %f\r",inbytes,outbytes,(float)outbytes/inbytes);
            //}
            if (outbytes - pout > 256) { p_flush(); } // compression failing
            pout = outbytes;
          } //inbytes

        } //for

/**/    put_char( min>>16);
/**/    put_char((min>>8) & 0xff);
/**/    put_char( min & 0x00ff);

        p_cleanup();

        return wbufpos;

      }
#endif // H_NODMCCOMPRESS

    //--------------------------------------------------

    int expand(unsigned char* outbuf, unsigned char* inbuf, int insize)
      {
        rbuf     = inbuf;
        rbufsize = insize;
        rbufpos  = 0;
        wbuf     = outbuf;
      //wbufsize = 0;
        wbufpos  = 0;
        int max = 0x1000000;
        int min = 0;
        // mid,
        int val;
        //    i,
        int inbytes=3;
        int pin=3;
        int outbytes=0;
        //    bit,
        //    c;

        p_init(memsize);

/**/    val  = get_char()<<16;
/**/    val += get_char()<<8;
/**/    val += get_char();
        while (1)
        {
          int c = 0;
          if (val == (max-1))
          {
            //fprintf(stderr,"expand: input %d output %d\n",inbytes,outbytes);
            break;
          }
          for (int i=0;i<8;i++)
          {
            int mid = min + (max-min-1)*predict();
            if (mid == min) mid++;
            if (mid == (max-1)) mid--;
            int bit;
            if (val >= mid)
            {
              bit = 1;
              min = mid;
            }
            else
            {
              bit = 0;
              max = mid;
            }
            p_update(bit != 0);
            c = c + c + bit;
            while ((max-min) < 256)
            {
              if (bit)max--;
              inbytes++;
            //val =  (val << 8) & 0xffff00  | (get_char()& 0xff); // warning: suggest parentheses around arithmetic in operand of ‘|’|
/**/          val = ((val << 8) & 0xffff00) | (get_char()& 0xff);
              min =  (min << 8) & 0xffff00;
              max = ((max << 8) & 0xffff00);
              if (min>=max) max = 0x1000000;
            } //max min
          } //for

/**/      put_char(c);

          if (!(++outbytes & 0xff))
          {
            if (inbytes - pin > 256) { p_flush(); } /* compression was failing */
            pin = inbytes;
          }

        } //while

        p_cleanup();
        return wbufpos;

      }

};

//----------------------------------------------------------------------
#endif
