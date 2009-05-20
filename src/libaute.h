/*
 * Copyright 2005-2006 Andrea Zagli <azagli@libero.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __AUTE_H__
#define __AUTE_H__

#include <glib.h>
#include <gmodule.h>

#include <libconfi.h>


G_BEGIN_DECLS


gchar *aute_autentica (Confi *confi);

GModule *aute_plugin_get_module (Confi *confi);


G_END_DECLS


#endif /* __AUTE_H__ */
