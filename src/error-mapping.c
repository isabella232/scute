/* error-mapping.c - Scute error mapping.
 * Copyright (C) 2006 g10 Code GmbH
 *
 * This file is part of Scute.
 *
 * Scute is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Scute is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <https://gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>

#include <gpg-error.h>

#include "cryptoki.h"
#include "debug.h"

#include "error-mapping.h"


/* Map a system error code to a cryptoki return value.  */
CK_RV
scute_sys_to_ck (int err)
{
  switch (err)
    {
    case 0:
      return CKR_OK;

    case ENOMEM:
      return CKR_HOST_MEMORY;

    default:
      /* CKR_GENERAL_ERROR is too strong.  */
      return CKR_FUNCTION_FAILED;
    }
}


/* Map a GnuPG error code to a cryptoki return value.  */
CK_RV
scute_gpg_err_to_ck (gpg_error_t err)
{
  if (err)
    DEBUG (DBG_CRIT, "Error occurred: %s (%s)\n", gpg_strerror (err),
	   gpg_strsource (err));

  switch (gpg_err_code (err))
    {
    case GPG_ERR_NO_ERROR:
      return CKR_OK;

    case GPG_ERR_NO_AGENT:
      return CKR_GENERAL_ERROR;

    case GPG_ERR_ENOMEM:
      return CKR_HOST_MEMORY;

    case GPG_ERR_BAD_PIN:
      return CKR_PIN_INCORRECT;

    case GPG_ERR_PIN_BLOCKED:
      return CKR_PIN_LOCKED;

    default:
      /* CKR_GENERAL_ERROR is too strong.  */
      return CKR_FUNCTION_FAILED;
    }
}
