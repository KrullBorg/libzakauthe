/*
 * Copyright (C) 2005-2016 Andrea Zagli <azagli@libero.it>
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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <stdio.h>

#include <glib.h>
#include <glib/gprintf.h>

#ifdef G_OS_WIN32
#include <windows.h>
#endif

#include <libpeas/peas.h>

#ifdef HAVE_LIBZAKCONFI
	#include <libzakconfi/libzakconfi.h>
#endif

#include "libzakauthe.h"

static gchar *pluginsdir;

static void zak_authe_class_init (ZakAutheClass *class);
static void zak_authe_init (ZakAuthe *form);

static void zak_authe_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_authe_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);
static void zak_authe_finalize (GObject *object);

#ifdef HAVE_LIBZAKCONFI
static gchar *zak_authe_get_pluggable_from_confi (ZakAuthe *authe);
#endif

#define ZAK_AUTHE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_TYPE_AUTHE, ZakAuthePrivate))

typedef struct _ZakAuthePrivate ZakAuthePrivate;
struct _ZakAuthePrivate
	{
		ZakAuthePluggable *pluggable;

		GSList *parameters;

#ifdef HAVE_LIBZAKCONFI
		ZakConfi *confi;
#endif
	};

G_DEFINE_TYPE (ZakAuthe, zak_authe, G_TYPE_OBJECT)

#ifdef G_OS_WIN32
static HMODULE backend_dll = NULL;

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

BOOL WINAPI
DllMain (HINSTANCE hinstDLL,
         DWORD     fdwReason,
         LPVOID    lpReserved)
{
	switch (fdwReason)
		{
			case DLL_PROCESS_ATTACH:
				backend_dll = (HMODULE) hinstDLL;
				break;
			case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:
				break;
		}
	return TRUE;
}
#endif

static void
zak_authe_class_init (ZakAutheClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_authe_set_property;
	object_class->get_property = zak_authe_get_property;
	object_class->finalize = zak_authe_finalize;

	g_type_class_add_private (object_class, sizeof (ZakAuthePrivate));
}

static void
zak_authe_init (ZakAuthe *form)
{
	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (form);

	priv->pluggable = NULL;
	priv->parameters = NULL;

#ifdef HAVE_LIBZAKCONFI
	priv->confi = NULL;
#endif
}

/**
 * zak_authe_new:
 *
 * Returns: the newly created #ZakAuthe object.
 */
ZakAuthe
*zak_authe_new ()
{
	ZakAuthe *zakauthe;

	#ifdef G_OS_WIN32

	gchar *moddir;
	gchar *p;

	moddir = g_win32_get_package_installation_directory_of_module (backend_dll);

	p = g_strrstr (moddir, g_strdup_printf ("%c", G_DIR_SEPARATOR));
	if (p != NULL
	    && (g_ascii_strcasecmp (p + 1, "src") == 0
	    || g_ascii_strcasecmp (p + 1, ".libs") == 0))
		{
			pluginsdir = g_strdup (LIB_ZAK_AUTHE_PLUGINS_DIR);
		}
	else
		{
			pluginsdir = g_build_filename (moddir, "lib", PACKAGE, "plugins", NULL);
			g_warning ("%s",pluginsdir);
		}

#undef PLUGINSDIR

#else

	pluginsdir = g_strdup (LIB_ZAK_AUTHE_PLUGINS_DIR);

#endif

	zakauthe = ZAK_AUTHE (g_object_new (zak_authe_get_type (), NULL));

	return zakauthe;
}

/*
 * zak_authe_set_config:
 * @zakauthe: an #ZakAuthe object.
 * @parameters: a #GSList of config parameters.
 *
 */
gboolean
zak_authe_set_config (ZakAuthe *zakauthe, GSList *parameters)
{
	gboolean ret;

	PeasEngine *peas_engine;

	gchar *module_name;

	const GList *lst_plugins;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (parameters != NULL && parameters->data != NULL, FALSE);

	priv->parameters = parameters;

	module_name = NULL;

#ifdef HAVE_LIBZAKCONFI
	/* the first and only parameters must be a ZakConfi object */
	if (g_strcmp0 ((gchar *)priv->parameters->data, "{libzakconfi}") == 0)
		{
			if (ZAK_IS_CONFI ((ZakConfi *)g_slist_nth_data (priv->parameters, 1)))
				{
					priv->confi = ZAK_CONFI ((ZakConfi *)g_slist_nth_data (priv->parameters, 1));
					module_name = zak_authe_get_pluggable_from_confi (zakauthe);
				}
		}
#endif

	if (module_name == NULL)
		{
			/* the first parameter must be the module's name with the full path */
			module_name = g_strdup ((gchar *)priv->parameters->data);
		}

	if (module_name == NULL)
		{
			/* didn't find valid parameters */
			return FALSE;
		}

	peas_engine = peas_engine_get_default ();
	if (peas_engine == NULL)
		{
			return FALSE;
		}

	ret = TRUE;

	peas_engine_add_search_path (peas_engine, pluginsdir, NULL);

	lst_plugins = peas_engine_get_plugin_list (peas_engine);
	while (lst_plugins)
		{
			PeasPluginInfo *ppinfo;

			ppinfo = (PeasPluginInfo *)lst_plugins->data;

			if (g_strcmp0 (module_name, peas_plugin_info_get_module_name (ppinfo)) == 0)
				{
					if (peas_engine_load_plugin (peas_engine, ppinfo))
						{
							PeasExtension *ext;
							ext = peas_engine_create_extension (peas_engine, ppinfo, ZAK_AUTHE_TYPE_PLUGGABLE,
							                                    NULL);
							priv->pluggable = (ZakAuthePluggable *)ext;
							break;
						}
				}

			lst_plugins = g_list_next (lst_plugins);
		}

	if (priv->pluggable == NULL)
		{
			g_warning ("No plugin found with name \"%s\".", module_name);
			ret = FALSE;
		}

	g_free (module_name);

	return ret;
}

/**
 * zak_authe_authe:
 * @zakauthe:
 *
 * Returns: the user name if authentication is successfull;
 * empty string ("") if it is clicked the "Cancel" button; #NULL on error.
 */
gchar
*zak_authe_authe (ZakAuthe *zakauthe)
{
	gchar *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (priv->pluggable != NULL, NULL);

	ret = NULL;

	/* calling plugin's function */
	ret = zak_authe_pluggable_authe (priv->pluggable, priv->parameters);

	return ret;
}

/**
 * zak_authe_get_password:
 * @zakauthe:
 * @password:
 *
 * Returns: il nome utente se l'autenticazione va a buon fine;
 * stringa vuota ("") se viene premuto "Annulla"; NULL in caso di errore.
 * Nel parametro @password ritorna la password inserita.
 */
gchar
*zak_authe_get_password (ZakAuthe *zakauthe, gchar **password)
{
	gchar *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (priv->pluggable != NULL, NULL);

	ret = NULL;

	/* calling plugin's function */
	ret = zak_authe_pluggable_authe_get_password (priv->pluggable, priv->parameters, password);

	return ret;
}

/**
 * zak_authe_authe_nogui:
 * @zakauthe:
 * @username:
 * @password:
 * @new_password:
 *
 * Returns: #TRUE if the user is authenticated; #FALSE otherwise.
 */
gboolean
zak_authe_authe_nogui (ZakAuthe *zakauthe, const gchar *username, const gchar *password, const gchar *new_password)
{
	gboolean ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (priv->pluggable != NULL, FALSE);

	/* calling plugin's function */
	ret = zak_authe_pluggable_authe_nogui (priv->pluggable, priv->parameters, username, password, new_password);

	return ret;
}

/**
 * zak_authe_get_management_gui:
 * @zakauthe:
 *
 */
GtkWidget
*zak_authe_get_management_gui (ZakAuthe *zakauthe)
{
	GtkWidget *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (priv->pluggable != NULL, NULL);

	ret = NULL;

	/* calling plugin's function */
	ret = zak_authe_pluggable_get_management_gui (priv->pluggable, priv->parameters);

	return ret;
}

/* PRIVATE */
static void
zak_authe_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakAuthe *aute = (ZakAuthe *)object;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
zak_authe_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakAuthe *aute = (ZakAuthe *)object;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
zak_authe_finalize (GObject *object)
{
	ZakAuthe *aute = ZAK_AUTHE (object);

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	/* Chain up to the parent class */
	G_OBJECT_CLASS (zak_authe_parent_class)->finalize (object);
}

#ifdef HAVE_LIBZAKCONFI
/**
 * zak_authe_get_pluggable_from_confi:
 * @authe: a #ZakAuthe object.
 *
 * Returns: plugin path.
 */
static gchar
*zak_authe_get_pluggable_from_confi (ZakAuthe *authe)
{
	gchar *libname;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (authe);

	g_return_val_if_fail (ZAK_IS_CONFI (priv->confi), NULL);

	libname = zak_confi_path_get_value (priv->confi, "libzakauthe/plugin");
	if (libname == NULL)
		{
			/* TO DO */
			g_warning ("No config libzakauthe/plugin value found.");
			return NULL;
		}

	return g_strdup (libname);
}
#endif
