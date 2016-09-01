/*
 * authepluggable.h
 * This file is part of libzakauthe
 *
 * Copyright (C) 2016 - Andrea Zagli <azagli@libero.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __ZAK_AUTHE_PLUGGABLE_H__
#define __ZAK_AUTHE_PLUGGABLE_H__

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define ZAK_AUTHE_TYPE_PLUGGABLE             (zak_authe_pluggable_get_type ())
#define ZAK_AUTHE_PLUGGABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ZAK_AUTHE_TYPE_PLUGGABLE, ZakAuthePluggable))
#define ZAK_AUTHE_PLUGGABLE_IFACE(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), ZAK_AUTHE_TYPE_PLUGGABLE, ZakAuthePluggableInterface))
#define ZAK_AUTHE_IS_PLUGGABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ZAK_AUTHE_TYPE_PLUGGABLE))
#define ZAK_AUTHE_PLUGGABLE_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ZAK_AUTHE_TYPE_PLUGGABLE, ZakAuthePluggableInterface))

/**
 * ZakAuthePluggable:
 *
 * Interface for pluggable plugins.
 */
typedef struct _ZakAuthePluggable           ZakAuthePluggable; /* dummy typedef */
typedef struct _ZakAuthePluggableInterface  ZakAuthePluggableInterface;

/**
 * ZakAuthePluggableInterface:
 * @g_iface: The parent interface.
 * @initialize: Construct the plugin.
 *
 * Provides an interface for pluggable plugins.
 */
struct _ZakAuthePluggableInterface {
	GTypeInterface g_iface;

	/* Virtual public methods */
	gchar *(*authe) (ZakAuthePluggable *pluggable, GSList *parameters);
	gchar *(*authe_get_password) (ZakAuthePluggable *pluggable, GSList *parameters, gchar **password);
	gboolean (*authe_nogui) (ZakAuthePluggable *pluggable, GSList *parameters, const gchar *username, const gchar *password, const gchar *new_password);
	GtkWidget *(*get_management_gui) (ZakAuthePluggable *pluggable, GSList *parameters);
};

/*
 * Public methods
 */
GType zak_authe_pluggable_get_type (void) G_GNUC_CONST;

gchar *zak_authe_pluggable_authe (ZakAuthePluggable *pluggable,
								  GSList *parameters);
gchar *zak_authe_pluggable_authe_get_password (ZakAuthePluggable *pluggable,
											   GSList *parameters,
											   gchar **password);
gboolean zak_authe_pluggable_authe_nogui (ZakAuthePluggable *pluggable,
										  GSList *parameters,
										  const gchar *username,
										  const gchar *password,
										  const gchar *new_password);
GtkWidget *zak_authe_pluggable_get_management_gui (ZakAuthePluggable *pluggable,
												   GSList *parameters);


G_END_DECLS

#endif /* __ZAK_AUTHE_PLUGGABLE_H__ */
