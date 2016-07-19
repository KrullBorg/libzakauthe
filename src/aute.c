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
#include <glib/gprintf.h>

#ifdef HAVE_LIBZAKCONFI
	#include <libzakconfi/libzakconfi.h>
#endif

#include "libzakauthe.h"

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

static gchar *zak_authe_get_module_from_confi (ZakAuthe *aute);

#define ZAK_AUTHE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_TYPE_AUTHE, ZakAuthePrivate))

typedef struct _ZakAuthePrivate ZakAuthePrivate;
struct _ZakAuthePrivate
	{
		GModule *module;

		GSList *parameters;

#ifdef HAVE_LIBZAKCONFI
		ZakConfi *confi;
#endif
	};

G_DEFINE_TYPE (ZakAuthe, zak_authe, G_TYPE_OBJECT)

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

	priv->module = NULL;
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
	return ZAK_AUTHE (g_object_new (zak_authe_get_type (), NULL));
}

/*
 * zak_authe_set_config:
 * @aute: an #ZakAuthe object.
 * @parameters: a #GSList of config parameters.
 *
 */
gboolean
zak_authe_set_config (ZakAuthe *aute, GSList *parameters)
{
	gchar *module_name;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

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
					module_name = zak_authe_get_module_from_confi (aute);
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

	/* loading library */
	priv->module = g_module_open (module_name, G_MODULE_BIND_LAZY);
	if (!priv->module)
		{
			/* TO DO */
			g_warning ("Error g_module_open.");
			return FALSE;
		}

	g_free (module_name);

	return TRUE;
}

/**
 * zak_authe_authe:
 * @aute:
 *
 * Returns: the user name if authentication is successfull;
 * empty string ("") if it is clicked the "Cancel" button; #NULL on error.
 */
gchar
*zak_authe_authe (ZakAuthe *aute)
{
	gchar *(*autentica) (GSList *parameters);
	gchar *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "zak_authe_plg_authe", (gpointer *)&autentica))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol\n");
			return NULL;
		}

	/* calling plugin's function */
	ret = (*autentica) (priv->parameters);

	return ret;
}

/**
 * zak_authe_get_password:
 * @aute:
 * @password:
 *
 * Returns: il nome utente se l'autenticazione va a buon fine;
 * stringa vuota ("") se viene premuto "Annulla"; NULL in caso di errore.
 * Nel parametro @password ritorna la password inserita.
 */
gchar
*zak_authe_get_password (ZakAuthe *aute, gchar **password)
{
	gchar *(*zak_authe_plg_get_password) (GSList *parameters, gchar **password);
	gchar *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "zak_authe_plg_get_password", (gpointer *)&zak_authe_plg_get_password))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol: zak_authe_plg_get_password.\n");

			/* try zak_authe_authe */
			ret = zak_authe_authe (aute);
		}
	else
		{
			/* calling plugin's function */
			ret = (*zak_authe_plg_get_password) (priv->parameters, password);
		}

	return ret;
}

/**
 * zak_authe_authe_nogui:
 * @zakaute:
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

	gboolean (*zak_authe_plg_authe_nogui) (GSList *parameters, const gchar *username, const gchar *password, const gchar *new_password);

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (zakauthe);

	g_return_val_if_fail (priv->module != NULL, FALSE);

	/* loading the function */
	if (!g_module_symbol (priv->module, "zak_authe_plg_authe_nogui", (gpointer *)&zak_authe_plg_authe_nogui))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol: zak_authe_plg_authe_nogui.\n");

			ret = FALSE;
		}
	else
		{
			/* calling plugin's function */
			ret = (*zak_authe_plg_authe_nogui) (priv->parameters, username, password, new_password);
		}

	return ret;
}

/**
 * zak_authe_get_management_gui:
 * @aute:
 *
 */
GtkWidget
*zak_authe_get_management_gui (ZakAuthe *aute)
{
	GtkWidget *(*get_management_gui) (GSList *parameters);
	GtkWidget *ret;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "zak_authe_plg_get_management_gui", (gpointer *)&get_management_gui))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol: zak_authe_plg_get_management_gui.\n");
			return NULL;
		}

	/* calling plugin's function */
	ret = (*get_management_gui) (priv->parameters);

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

	/* closing the library */
	if (priv->module != NULL)
		{
			if (!g_module_close (priv->module))
				{
					g_fprintf (stderr, "Error g_module_close\n");
				}
			else
				{
					priv->module = NULL;
				}
		}

	/* Chain up to the parent class */
	G_OBJECT_CLASS (zak_authe_parent_class)->finalize (object);
}

#ifdef HAVE_LIBZAKCONFI
/**
 * zak_authe_get_plugin_module:
 * @aute: a #ZakAuthe object.
 *
 * Returns: plugin path.
 */
static gchar
*zak_authe_get_module_from_confi (ZakAuthe *aute)
{
	gchar *libname;

	ZakAuthePrivate *priv = ZAK_AUTHE_GET_PRIVATE (aute);

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
