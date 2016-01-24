/*
 * Copyright 2005-2016 Andrea Zagli <azagli@libero.it>
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

#ifndef __ZAKAUTHE_H__
#define __ZAKAUTHE_H__

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ZAK_TYPE_AUTHE                 (zak_authe_get_type ())
#define ZAK_AUTHE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ZAK_TYPE_AUTHE, ZakAuthe))
#define ZAK_AUTHE_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), ZAK_TYPE_AUTHE, ZakAutheClass))
#define ZAK_IS_AUTHE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ZAK_TYPE_AUTHE))
#define ZAK_IS_AUTHE_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), ZAK_TYPE_AUTHE))
#define ZAKAUTHE_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), ZAK_TYPE_AUTHE, ZakAutheClass))


typedef struct _ZakAuthe ZakAuthe;
typedef struct _ZakAutheClass ZakAutheClass;

struct _ZakAuthe
	{
		GObject parent;
	};

struct _ZakAutheClass
	{
		GObjectClass parent_class;
	};

GType zak_authe_get_type (void) G_GNUC_CONST;

ZakAuthe *zak_authe_new (void);

gboolean zak_authe_set_config (ZakAuthe *zakauthe, GSList *parameters);

gchar *zak_authe_authe (ZakAuthe *zakauthe);
gchar *zak_authe_get_password (ZakAuthe *zakauthe, gchar **password);
gboolean zak_authe_authe_nogui (ZakAuthe *zakauthe, const gchar *username, const gchar *password, const gchar *new_password);

GtkWidget *zak_authe_get_management_gui (ZakAuthe *zakauthe);


G_END_DECLS


#endif /* __ZAKAUTHE_H__ */
