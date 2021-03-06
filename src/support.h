/* support.h - Cryptoki implementation support functions.
   Copyright (C) 2006 g10 Code GmbH

   This file is part of Scute.

   Scute is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Scute is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Scute; if not, write to the Free Software Foundation,
   Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

   In addition, as a special exception, g10 Code GmbH gives permission
   to link this library: with the Mozilla Foundation's code for
   Mozilla (or with modified versions of it that use the same license
   as the "Mozilla" code), and distribute the linked executables.  You
   must obey the GNU General Public License in all respects for all of
   the code used other than "Mozilla".  If you modify this file, you
   may extend this exception to your version of the file, but you are
   not obligated to do so.  If you do not wish to do so, delete this
   exception statement from your version.  */

#ifndef SUPPORT_H
#define SUPPORT_H	1

#include <gpg-error.h>

#define spacep(p)   (*(p) == ' ' || *(p) == '\t')
#define digitp(p)   (*(p) >= '0' && *(p) <= '9')
#define hexdigitp(a) (digitp (a)			\
                      || (*(a) >= 'A' && *(a) <= 'F')	\
                      || (*(a) >= 'a' && *(a) <= 'f'))
#define xtoi_1(p)   (*(p) <= '9'? (*(p)- '0'): \
                     *(p) <= 'F'? (*(p)-'A'+10):(*(p)-'a'+10))
#define xtoi_2(p)   ((xtoi_1(p) * 16) + xtoi_1((p)+1))

#define DIM(x) (sizeof (x) / sizeof (x[0]))

/* Copy a string into its location, with blank character padding.  */
static inline void
scute_copy_string (char *dest, const char *src, int max_len)
{
  int i;
  for (i = 0; (i < max_len) && (*src != '\0'); i++)
    *(dest++) = *(src++);
  while (i++ < max_len)
    *(dest++) = ' ';
}

/* Use gpg-errors printf functions for better portability.  We also
   replace the standard snprintf with our implementation due to a bug
   in some mingw32 versions related to the 'l' format modifier.  */
#define asprintf  gpgrt_asprintf
#define vasprintf gpgrt_vasprintf
#define snprintf  gpgrt_snprintf

/*-- Simple replacement functions. */
#ifndef HAVE_TTYNAME
/* Systems without ttyname (W32) will merely return NULL. */
static inline char *
ttyname (int fd)
{
  (void)fd;
  return NULL;
}
#endif /* !HAVE_TTYNAME */


gpg_error_t read_first_line (const char *command, char *buffer, size_t bufsize);
const char *get_gpgconf_path (void);
int get_gnupg_version (int *minor);
int is_gnupg_older_than (int major, int minor, int micro);
const char *get_gpgsm_path (void);



#endif	/* !SUPPORT_H */
