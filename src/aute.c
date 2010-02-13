/*
 * Copyright (C) 2005-2010 Andrea Zagli <azagli@libero.it>
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

#include "libaute.h"

static void aute_class_init (AuteClass *class);
static void aute_init (Aute *form);

static void aute_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void aute_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);
static void aute_finalize (GObject *object);

GModule *aute_get_module_from_confi (Aute *aute);

#define AUTE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TYPE_AUTE, AutePrivate))

typedef struct _AutePrivate AutePrivate;
struct _AutePrivate
	{
		GModule *module;

		GSList *parameters;

#ifdef HAVE_LIBCONFI
		Confi *confi;
#endif
	};

G_DEFINE_TYPE (Aute, aute, G_TYPE_OBJECT)

static void
aute_class_init (AuteClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = aute_set_property;
	object_class->get_property = aute_get_property;
	object_class->finalize = aute_finalize;

	g_type_class_add_private (object_class, sizeof (AutePrivate));
}

static void
aute_init (Aute *form)
{
	AutePrivate *priv = AUTE_GET_PRIVATE (form);

	priv->module = NULL;
	priv->parameters = NULL;

#ifdef HAVE_LIBCONFI
	priv->confi = NULL;
#endif
}

/**
 * aute_new:
 *
 * Returns: the newly created #Aute object.
 */
Aute
*aute_new ()
{
	return AUTE (g_object_new (aute_get_type (), NULL));
}

/*
 * aute_set_config:
 * @aute: an #Aute object.
 * @parameters: a #GSList of config parameters.
 *
 */
gboolean
aute_set_config (Aute *aute, GSList *parameters)
{
	gchar *module_name;

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

	g_return_val_if_fail (parameters != NULL && parameters->data != NULL, FALSE);

	priv->parameters = parameters;

	module_name = NULL;

#ifdef HAVE_LIBCONFI
	/* the first and only parameters must be a Confi object */
	Confi *confi;

	if (IS_CONFI (priv->parameters->data))
		{
			priv->confi = CONFI (priv->parameters->data);
			module_name = aute_get_module_from_confi (aute);
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
 * aute_autentica:
 * @confi: un oggetto #Confi; se viene passato NULL verrà utilizzata la 
 * configurazione Default, letta dal database specificato in GConf dell'utente
 * corrente.
 *
 * Returns: il nome utente se l'autenticazione va a buon fine; 
 * stringa vuota ("") se viene premuto "Annulla"; NULL in caso di errore.
 */
gchar
*aute_autentica (Aute *aute)
{
	gchar *(*autentica) (GSList *parameters);
	gchar *ret;

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

	g_return_val_if_fail (priv->module != NULL, NULL);

	ret = NULL;

	/* loading the function */
	if (!g_module_symbol (priv->module, "autentica", (gpointer *)&autentica))
		{
			/* TO DO */
			g_fprintf (stderr, "Error g_module_symbol\n");
			return NULL;
		}

	/* calling plugin's function */
	ret = (*autentica) (priv->parameters);

	return ret;
}

/* PRIVATE */
static void
aute_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	Aute *aute = (Aute *)object;

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
aute_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	Aute *aute = (Aute *)object;

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
	  }
}

static void
aute_finalize (GObject *object)
{
	Aute *aute = AUTE (object);

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

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
	G_OBJECT_CLASS (aute_parent_class)->finalize (object);
}

#ifdef HAVE_LIBCONFI
/**
 * aute_get_plugin_module:
 * @aute: un oggetto #Aute.
 *
 * Returns: il nome, con il percorso, del plugin.
 */
gchar
*aute_get_module_from_confi (Aute *aute)
{
	gchar *libname;

	AutePrivate *priv = AUTE_GET_PRIVATE (aute);

	g_return_val_if_fail (IS_CONFI (priv->confi), NULL);

	libname = confi_path_get_value (priv->confi, "aute/plugin");
	if (libname == NULL)
		{
			/* TO DO */
			g_warning ("Valore nullo della configurazione per il plugin.");
			return NULL;
		}
	libname = g_strconcat (LIBAUTE_PLUGINS_DIR "/", libname, NULL);

	return libname;
}
#endif
