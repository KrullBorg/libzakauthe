/*
 * Copyright (C) 2005-2006 Andrea Zagli <azagli@libero.it>
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

#include <stdio.h>

#include "libaute.h"

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
*aute_autentica (Confi *confi)
{
	GModule *module;

	gchar *(*autentica) (Confi *confi);
	gchar *ret;

	if (confi == NULL)
		{
			/* TO DO */
			/* leggere il provider_id e cnc_string da GConf dell'utente */
			confi = confi_new (NULL, "PostgreSQL", "HOSTADDR=127.0.0.1;PORT=5432;DATABASE=confi;HOST=localhost;USER=postgres", "Default", NULL, FALSE);
			if (confi == NULL)
				{
					/* TO DO */
					return NULL;
				}
		}

	/* caricamento del plugin in base alla configurazione */
	module = aute_plugin_get_module (confi);

	/* carico la funzione */
	if (!g_module_symbol (module, "autentica", (gpointer *)&autentica))
		{
			/* TO DO */
			g_fprintf (stderr, "Error g_module_symbol\n");
			return NULL;
		}

	/* chiamo la funzione del plugin */
	ret = (*autentica) (confi);

	/* chiudo la libreria */
	if (!g_module_close (module))
		{
			g_fprintf (stderr, "Error g_module_close\n");
		}

	return ret;
}

/**
 * aute_get_plugin_module:
 * @confi: un oggetto #Confi.
 *
 * Returns: l'oggetto #GModule corrispondente al plugin.
 */
GModule
*aute_plugin_get_module (Confi *confi)
{
	gchar *libname;
	GModule *module = NULL;

	libname = confi_path_get_value (confi, "aute/plugin");
	if (libname == NULL)
		{
			/* TO DO */
			g_warning ("Valore nullo della configurazione per il plugin.");
			return NULL;
		}
	libname = g_strconcat (LIBAUTE_PLUGINS_DIR "/", libname, NULL);

	/* carico la libreria */
	module = g_module_open (libname, G_MODULE_BIND_LAZY);
	if (!module)
		{
			/* TO DO */
			g_warning ("Error g_module_open.");
			return NULL;
		}

	return module;
}
