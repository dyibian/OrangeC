/* 
   Copyright 1994-2003 Free Software Foundation, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 
   USA

   You may contact the author at:

   mailto::camille@bluegrass.net

   or by snail mail at:

   David Lindauer
   850 Washburn Ave Apt 99
   Louisville, KY 40222
*/
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <libp.h>

int _readbuf(FILE *stream) ;

size_t _RTL_FUNC fread(void *buf, size_t size, size_t count, FILE *stream)
{
	int i = 0,rv, num = size * count;
	char *out = (char *)buf ;
	if (stream->token != FILTOK) {
		errno = _dos_errno = ENOENT;
		return 0;
	}
	if (!(stream->flags & _F_READ)) {
		stream->flags |= _F_ERR;
		errno = EFAULT;
		return 0;
	}
	if (stream == stdin)
		fflush(stdout);
   if (!(stream->flags & _F_IN)) {
		if (stream->flags & _F_OUT) {
			if (fflush(stream))
				return 0;
		}
		stream->flags &= ~_F_OUT;
		stream->flags |= _F_IN;
		stream->level = 0;
	}
	if (stream->flags & _F_EOF)
		return 0;
	if (num == 0)
		return 0 ;
	if (stream->hold) {
		out[i++] = stream->hold ;
		stream->hold = 0;
	}
	for (;i < num;) {
		if (--stream->level <= 0) {
	      if (stream->flags & _F_BUFFEREDSTRING) {
	         stream->flags |= _F_EOF ;
	         break ;
	      }
			rv = _readbuf(stream);
			if (rv)
				break ;
		}
		rv = *stream->curp++ ;
		stream->hold = 0 ;
		if (rv != '\r' || (stream->flags & _F_BIN))
			out[i++] = rv ;
	}
	return i / size ;
}