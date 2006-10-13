/* agent.c - Talking to gpg-agent.
   Copyright (C) 2006 g10 Code GmbH

   This file is part of Scute[1].

   [1] Derived from the RSA Security Inc. PKCS #11 Cryptographic Token
   Interface (Cryptoki).
 
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

#ifndef AGENT_H
#define AGENT_H	1

#include <gpg-error.h>


/* The information structure for a smart card.  */
struct agent_card_info_s 
{
  int error;		/* Private.  */
  char *serialno;	/* Malloced hex string.  */
  char *disp_name;	/* Malloced.  */
  char *disp_lang;	/* Malloced.  */
  int  disp_sex;	/* 0 = unspecified, 1 = male, 2 = female.  */
  char *pubkey_url;	/* Malloced.  */
  char *login_data;	/* Malloced.  */
  char *private_do[4];	/* Malloced.  */
  char cafpr1valid;
  char cafpr2valid;
  char cafpr3valid;
  char cafpr1[20];
  char cafpr2[20];
  char cafpr3[20];
  char fpr1valid;
  char fpr2valid;
  char fpr3valid;
  char fpr1[20];
  char fpr2[20];
  char fpr3[20];
  unsigned int fpr1time;
  unsigned int fpr2time;
  unsigned int fpr3time;
  unsigned long sig_counter;
  int chv1_cached;	/* True if a PIN is not required for each
			   signing.  Note that the gpg-agent might
			   cache it anyway.  */
  int chvmaxlen[3];	/* Maximum allowed length of a CHV.  */
  int chvretry[3];	/* Allowed retries for the CHV; 0 = blocked.  */
  char grip1valid;
  char grip2valid;
  char grip3valid;
  char grip1[41];
  char grip2[41];
  char grip3[41];
};


/* Try to connect to the agent via socket or fork it off and work by
   pipes.  Handle the server's initial greeting.  */
gpg_error_t scute_agent_initialize (void);

void scute_agent_finalize (void);

/* Release the card info structure INFO.  */
void agent_release_card_info (struct agent_card_info_s *info);

/* Call the agent to learn about a smartcard.  */
gpg_error_t agent_learn (struct agent_card_info_s *info);

/* Call the agent to learn about a smartcard.  */
gpg_error_t agent_check_status (void);

gpg_error_t agent_sign (char *grip, unsigned char *data, int len,
			unsigned char *sig_result, unsigned int *sig_len);

#endif	/* AGENT_H */
