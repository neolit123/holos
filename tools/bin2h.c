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

#include <stdio.h>

#define NUM 16

int main (int argc, char **argv)
{
	FILE *in, *out;
	unsigned char a[NUM];
	unsigned int i,c,size = 0;

	if (argc!=4)
	{
		fprintf(stderr,"usage: bin2h inputfile output.h name[]\n");
		return 1;
	}
	if (!(in=fopen(argv[1],"rb")))
	{
		fprintf(stderr,"error: can't open %s.\n",argv[1]);
		return 1;
	}
	if (!(out=fopen(argv[2],"wt")))
	{
		fclose(in);
		fprintf(stderr,"error: can't create %s.\n",argv[2]);
		return 1;
	}

	if (argv[3] == NULL)
	{
		printf("error: name[] required\n");
		return 1;
	}

	fseek (in, 0, SEEK_END);
	size = ftell(in);
	fseek (in, 0, SEEK_SET);

	fprintf (out,"const unsigned %s_size = %u;\n", argv[3], size);
	fprintf (out,"const unsigned char %s[] = \n{\n", argv[3]);
	while ((c=fread(a,1,NUM,in))!=0)
	{
		fprintf(out,"  ");
		if (c == NUM)
		{
			for (i=0; i<c; i++) fprintf(out,"0x%02x, ",a[i]);
			fprintf(out,"\n");
		} else
		{
			for (i=0; i<c-1; i++) fprintf(out,"0x%02x, ",a[i]);
			fprintf(out,"0x%02x\n",a[c-1]);
		}
	}
	fprintf(out,"};\n");
	fprintf(out,"// '%s' : %i\n",argv[1],size);
	fclose (out);
	fclose (in);
	return 0;
}
