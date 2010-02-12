/*
 * Copyright 2005-2010 Andrea Zagli <azagli@libero.it>
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
#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define TYPE_AUTE                 (aute_get_type ())
#define AUTE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_AUTE, Aute))
#define AUTE_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_AUTE, AuteClass))
#define IS_AUTE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_AUTE))
#define IS_AUTE_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_AUTE))
#define AUTE_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_AUTE, AuteClass))


typedef struct _Aute Aute;
typedef struct _AuteClass AuteClass;

struct _Aute
	{
		GObject parent;
	};

struct _AuteClass
	{
		GObjectClass parent_class;
	};

GType aute_get_type (void) G_GNUC_CONST;

Aute *aute_new (void);

gboolean aute_set_config (Aute *aute, GSList *parameters);

gchar *aute_autentica (Aute *aute);


G_END_DECLS


#endif /* __AUTE_H__ */
