/*
 * Copyright (C) 2005-2015 Andrea Zagli <azagli@libero.it>
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

#include "libzakauth.h"

static void zak_auth_class_init (ZakAuthClass *class);
static void zak_auth_init (ZakAuth *form);

static void zak_auth_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_auth_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);
static void zak_auth_finalize (GObject *object);

GModule *zak_auth_get_module_from_confi (ZakAuth *aute);

#define ZAK_AUTH_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_TYPE_AUTH, ZakAuthPrivate))

typedef struct _ZakAuthPrivate ZakAuthPrivate;
struct _ZakAuthPrivate
	{
		GModule *module;

		GSList *parameters;

#ifdef HAVE_LIBCONFI
		Confi *confi;
#endif
	};

G_DEFINE_TYPE (ZakAuth, zak_auth, G_TYPE_OBJECT)

static void
zak_auth_class_init (ZakAuthClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_auth_set_property;
	object_class->get_property = zak_auth_get_property;
	object_class->finalize = zak_auth_finalize;

	g_type_class_add_private (object_class, sizeof (ZakAuthPrivate));
}

static void
zak_auth_init (ZakAuth *form)
{
	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (form);

	priv->module = NULL;
	priv->parameters = NULL;

#ifdef HAVE_LIBCONFI
	priv->confi = NULL;
#endif
}

/**
 * zak_auth_new:
 *
 * Returns: the newly created #ZakAuth object.
 */
ZakAuth
*zak_auth_new ()
{
	return ZAK_AUTH (g_object_new (zak_auth_get_type (), NULL));
}

/*
 * zak_auth_set_config:
 * @aute: an #ZakAuth object.
 * @parameters: a #GSList of config parameters.
 *
 */
gboolean
zak_auth_set_config (ZakAuth *aute, GSList *parameters)
{
	gchar *module_name;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	g_return_val_if_fail (parameters != NULL && parameters->data != NULL, FALSE);

	priv->parameters = parameters;

	module_name = NULL;

#ifdef HAVE_LIBCONFI
	/* the first and only parameters must be a Confi object */
	Confi *confi;

	if (IS_CONFI (priv->parameters->data))
		{
			priv->confi = CONFI (priv->parameters->data);
			module_name = zak_auth_get_module_from_confi (aute);
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

	return TRUE;
}

/**
 * zak_auth_auth:
 * @aute:
 *
 * Returns: il nome utente se l'autenticazione va a buon fine;
 * stringa vuota ("") se viene premuto "Annulla"; NULL in caso di errore.
 */
gchar
*zak_auth_auth (ZakAuth *aute)
{
	gchar *(*autentica) (GSList *parameters);
	gchar *ret;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "autentica", (gpointer *)&autentica))
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
 * zak_auth_get_password:
 * @aute:
 * @password:
 *
 * Returns: il nome utente se l'autenticazione va a buon fine;
 * stringa vuota ("") se viene premuto "Annulla"; NULL in caso di errore.
 * Nel parametro @password ritorna la password inserita.
 */
gchar
*zak_auth_get_password (ZakAuth *aute, gchar **password)
{
	gchar *(*zak_auth_plg_get_password) (GSList *parameters, gchar **password);
	gchar *ret;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "zak_auth_plg_get_password", (gpointer *)&zak_auth_plg_get_password))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol: zak_auth_plg_get_password.\n");

			/* try zak_auth_auth */
			ret = zak_auth_auth (aute);
		}
	else
		{
			/* calling plugin's function */
			ret = (*zak_auth_plg_get_password) (priv->parameters, password);
		}

	return ret;
}

/**
 * zak_auth_get_management_gui:
 * @aute:
 *
 */
GtkWidget
*zak_auth_get_management_gui (ZakAuth *aute)
{
	GtkWidget *(*get_management_gui) (GSList *parameters);
	GtkWidget *ret;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "get_management_gui", (gpointer *)&get_management_gui))
		{
			/* TO DO */
			g_warning ("Error g_module_symbol\n");
			return NULL;
		}

	/* calling plugin's function */
	ret = (*get_management_gui) (priv->parameters);

	return ret;

}

/* PRIVATE */
static void
zak_auth_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakAuth *aute = (ZakAuth *)object;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
zak_auth_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakAuth *aute = (ZakAuth *)object;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
zak_auth_finalize (GObject *object)
{
	ZakAuth *aute = ZAK_AUTH (object);

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

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
	G_OBJECT_CLASS (zak_auth_parent_class)->finalize (object);
}

#ifdef HAVE_LIBCONFI
/**
 * zak_auth_get_plugin_module:
 * @aute: un oggetto #ZakAuth.
 *
 * Returns: il nome, con il percorso, del plugin.
 * Returns: il nome, con il percorso, del plugin.
 */
gchar
*zak_auth_get_module_from_confi (ZakAuth *aute)
{
	gchar *libname;

	ZakAuthPrivate *priv = ZAK_AUTH_GET_PRIVATE (aute);

	g_return_val_if_fail (IS_CONFI (priv->confi), NULL);

	libname = confi_path_get_value (priv->confi, "aute/plugin");
	if (libname == NULL)
		{
			/* TO DO */
			g_warning ("Valore nullo della configurazione per il plugin.");
			return NULL;
		}
	libname = g_strconcat (LIB_ZAK_AUTH_PLUGINS_DIR "/", libname, NULL);

	return libname;
}
#endif
