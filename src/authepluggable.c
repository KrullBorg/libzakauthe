/*
 * authepluggable.c
 * This file is part of libzakauthe
 *
 * Copyright (C) 2016 Andrea Zagli <azagli@libero.it>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "authepluggable.h"

/**
 * SECTION:authepluggable
 * @short_description: Interface for pluggable plugins.
 * @see_also: #PeasExtensionSet
 *
 **/

G_DEFINE_INTERFACE(ZakAuthePluggable, zak_authe_pluggable, G_TYPE_OBJECT)

void
zak_authe_pluggable_default_init (ZakAuthePluggableInterface *iface)
{
	static gboolean initialized = FALSE;

	if (!initialized)
		{
			initialized = TRUE;
		}
}

/**
 * zak_authe_pluggable_authe:
 * @pluggable: A #ZakAuthePluggable.
 * @parameters:
 *
 * Returns:
 */
gchar
*zak_authe_pluggable_authe (ZakAuthePluggable *pluggable, GSList *parameters)
{
	ZakAuthePluggableInterface *iface;

	g_return_val_if_fail (ZAK_AUTHE_IS_PLUGGABLE (pluggable), FALSE);

	iface = ZAK_AUTHE_PLUGGABLE_GET_IFACE (pluggable);
	g_return_val_if_fail (iface->authe != NULL, FALSE);

	return iface->authe (pluggable, parameters);
}

/**
 * zak_authe_pluggable_authe_get_password:
 * @pluggable: A #ZakAuthePluggable.
 * @parameters:
 * @password:
 *
 * Returns:
*/
gchar
*zak_authe_pluggable_authe_get_password (ZakAuthePluggable *pluggable, GSList *parameters, gchar **password)
{
	ZakAuthePluggableInterface *iface;

	g_return_val_if_fail (ZAK_AUTHE_IS_PLUGGABLE (pluggable), FALSE);

	iface = ZAK_AUTHE_PLUGGABLE_GET_IFACE (pluggable);
	g_return_val_if_fail (iface->authe_get_password != NULL, FALSE);

	return iface->authe_get_password (pluggable, parameters, password);
}

/**
 * zak_authe_pluggable_authe_nogui:
 * @pluggable: A #ZakAuthePluggable.
 * @parameters:
 * @username:
 * @password:
 * @new_password:
 *
 * Returns:
*/
gboolean
zak_authe_pluggable_authe_nogui (ZakAuthePluggable *pluggable, GSList *parameters, const gchar *username, const gchar *password, const gchar *new_password)
{
	ZakAuthePluggableInterface *iface;

	g_return_val_if_fail (ZAK_AUTHE_IS_PLUGGABLE (pluggable), FALSE);

	iface = ZAK_AUTHE_PLUGGABLE_GET_IFACE (pluggable);
	g_return_val_if_fail (iface->authe_nogui != NULL, FALSE);

	return iface->authe_nogui (pluggable, parameters, username, password, new_password);
}

/**
 * zak_authe_pluggable_get_management_gui:
 * @pluggable: a #ZakAuthePluggable object.
 * @parameters:
 *
 * Returns:
 */
GtkWidget
*zak_authe_pluggable_get_management_gui (ZakAuthePluggable *pluggable, GSList *parameters)
{
	ZakAuthePluggableInterface *iface;

	g_return_val_if_fail (ZAK_AUTHE_IS_PLUGGABLE (pluggable), FALSE);

	iface = ZAK_AUTHE_PLUGGABLE_GET_IFACE (pluggable);
	g_return_val_if_fail (iface->get_management_gui != NULL, FALSE);

	return iface->get_management_gui (pluggable, parameters);
}
