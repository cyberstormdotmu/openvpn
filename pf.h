/*
 *  OpenVPN -- An application to securely tunnel IP networks
 *             over a single TCP/UDP port, with support for SSL/TLS-based
 *             session authentication and key exchange,
 *             packet encryption, packet authentication, and
 *             packet compression.
 *
 *  Copyright (C) 2002-2005 OpenVPN Solutions LLC <info@openvpn.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (see the file COPYING included with this
 *  distribution); if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* packet filter functions */

#if defined(ENABLE_PF) && !defined(OPENVPN_PF_H)
#define OPENVPN_PF_H

#include "list.h"
#include "mroute.h"

#define PF_DEBUG 0

struct context;

struct ipv4_subnet {
  bool exclude;
  in_addr_t network;
  in_addr_t netmask;
};

struct pf_subnet {
  struct pf_subnet *next;
  struct ipv4_subnet rule;
};

struct pf_subnet_set {
  bool default_allow;
  struct pf_subnet *list;
};

struct pf_cn {
  bool exclude;
  char *cn;
};

struct pf_cn_elem {
  struct pf_cn_elem *next;
  struct pf_cn rule;
};

struct pf_cn_set {
  bool default_allow;
  struct pf_cn_elem *list;
  struct hash *hash_table;
};

struct pf_set {
  bool kill;
  struct pf_subnet_set sns;
  struct pf_cn_set cns;
};

struct pf_context {
  char *filename;
  time_t file_last_mod;
  unsigned int n_check_reload;
  struct event_timeout reload;
  struct pf_set *pfs;
};

void pf_init_context (struct context *c);

void pf_destroy_context (struct pf_context *pfc);

void pf_check_reload (struct context *c);

bool pf_c2c_test (const struct context *src, const struct context *dest);

bool pf_addr_test (const struct context *src, const struct mroute_addr *dest);

static inline bool
pf_kill_test (const struct pf_set *pfs)
{
  return pfs->kill;
}

#if PF_DEBUG >= 1
void pf_context_print (const struct pf_context *pfc, const char *prefix, const int lev);
#endif

#endif
