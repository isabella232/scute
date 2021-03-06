/* p11-getmechanismlist.c - Cryptoki implementation.
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

#include "cryptoki.h"

#include "locking.h"
#include "slots.h"


CK_RV CK_SPEC
C_GetMechanismList (CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList,
                    CK_ULONG_PTR pulCount)
{
  CK_RV err = CKR_OK;
  CK_ULONG left;
  slot_iterator_t slot;
  mechanism_iterator_t mechanism;

  if (pulCount == NULL_PTR)
    return CKR_ARGUMENTS_BAD;

  err = scute_global_lock ();
  if (err)
    return err;

  err = slots_lookup (slotID, &slot);
  if (err)
    goto out;

  /* Leave LEFT positive for the whole search when only counting.  */
  left = pMechanismList ? *pulCount : 1;
  *pulCount = 0;
  err = mechanisms_iterate_first (slot, &mechanism);
  if (err)
    goto out;

  while (!mechanisms_iterate_last (slot, &mechanism) && left && !err)
    {
      (*pulCount)++;

      if (pMechanismList)
	{
	  *(pMechanismList++) = mechanism_get_type (slot, mechanism);
	  left--;
	}
      err = mechanisms_iterate_next (slot, &mechanism);
    }

  if (err)
    goto out;

  if (!mechanisms_iterate_last (slot, &mechanism) && !left)
    {
      err = CKR_BUFFER_TOO_SMALL;
      goto out;
    }

 out:
  scute_global_unlock ();
  return err;
}
