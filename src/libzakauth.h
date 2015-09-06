/*
 * Copyright 2005-2015 Andrea Zagli <azagli@libero.it>
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

#ifndef __ZAKAUTH_H__
#define __ZAKAUTH_H__

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ZAK_TYPE_AUTH                 (zak_auth_get_type ())
#define ZAK_AUTH(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ZAK_TYPE_AUTH, ZakAuth))
#define ZAK_AUTH_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), ZAK_TYPE_AUTH, ZakAuthClass))
#define ZAK_IS_AUTH(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ZAK_TYPE_AUTH))
#define ZAK_IS_AUTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), ZAK_TYPE_AUTH))
#define ZAKAUTH_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), ZAK_TYPE_AUTH, ZakAuthClass))


typedef struct _ZakAuth ZakAuth;
typedef struct _ZakAuthClass ZakAuthClass;

struct _ZakAuth
	{
		GObject parent;
	};

struct _ZakAuthClass
	{
		GObjectClass parent_class;
	};

GType zak_auth_get_type (void) G_GNUC_CONST;

ZakAuth *zak_auth_new (void);

gboolean zak_auth_set_config (ZakAuth *zakauth, GSList *parameters);

gchar *zak_auth_auth (ZakAuth *zakauth);
gchar *zak_auth_get_password (ZakAuth *zakauth, gchar **password);

GtkWidget *zak_auth_get_management_gui (ZakAuth *zakauth);


G_END_DECLS


#endif /* __ZAKAUTH_H__ */
