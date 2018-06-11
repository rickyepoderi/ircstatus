/*
 * ircaway - changes the nick in irc when changing status
 * Copyright (C) 2004 rickyepoderi <rickyepoderi@yahoo.es>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1301, USA.
 */

#define PURPLE_PLUGINS

#include <glib.h>
#include <notify.h>
#include <plugin.h>
#include <version.h>
#include <debug.h>
#include <savedstatuses.h>
#include <cmds.h>
#include <accountopt.h>

#include <libintl.h>
#include <locale.h>
#define _(String) dgettext(PACKAGE, String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

#define NAME "ircstatus"
#define IRC_ID "prpl-irc"
#define ID "es.rickyepoderi." NAME
#define IRC_VERSION "0.0.1"
#define DEFAULT_SEPARATOR "|"
#define ALPHANUMERIC "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890"

#define IRCSTATUS_PREF_ROOT "/plugins/core/ircstatus"
#define IRCSTATUS_PREF_SEPARATOR "/plugins/core/ircstatus/separator"
#define IRCSTATUS_PREF_ACCOUNT "/plugins/core/ircstatus/account"

static gboolean plugin_load(PurplePlugin *plugin);
static gboolean plugin_unload(PurplePlugin *plugin);
static void status_changed(PurpleAccount *account, PurpleStatus *status);
PurplePluginPrefFrame* get_prefs_frame(PurplePlugin*);

static PurplePluginUiInfo prefs = {
	get_prefs_frame,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,
	ID,
	N_("IRC Status"),
	IRC_VERSION,
	N_("IRC Status"),
	N_("Changes your nick when changing your status."),
	"rickyepoderi",
	N_("https://github.com/rickyepoderi/ircstatus"),
	plugin_load,
	plugin_unload,
	NULL,
	NULL,
	NULL,
	&prefs,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static PurpleConversation *dummy_conversation(PurpleAccount *account) {
	PurpleConversation *dummy;
	dummy = g_new0(PurpleConversation, 1);
	dummy->type = PURPLE_CONV_TYPE_IM;
	dummy->account = account;
	return dummy;
}

static void change_nick(PurpleAccount *account, char *new_nick) {

	PurpleConversation *conversation;
	char *nick_command;
	char *error;

	if (new_nick == NULL)
		return;

	conversation = dummy_conversation(account);
	nick_command = g_strconcat("nick ", new_nick, NULL);

	if (purple_cmd_do_command(conversation, nick_command, nick_command, &error) != PURPLE_CMD_STATUS_OK) {
		purple_debug_warning(NAME, "Failed to execute %s\n", nick_command);
		g_free(error);
	}

	g_free(conversation);
	g_free(nick_command);
}

static void status_changed_cb(PurpleSavedStatus *saved_status) {
	PurpleAccount *account;
        const char *old_nick;
        char *new_nick;
        char **username;
	char *status_title;
        const char *separator;
        const char *account_name;

	separator = purple_prefs_get_string(IRCSTATUS_PREF_SEPARATOR);
	account_name = purple_prefs_get_string(IRCSTATUS_PREF_ACCOUNT);
	if (account_name != NULL) {
		account = purple_accounts_find(account_name, IRC_ID);
		if (account != NULL && purple_account_is_connected(account)) {
        		status_title = g_strdup(purple_savedstatus_get_title(saved_status));
			status_title = g_strcanon(status_title, ALPHANUMERIC, '_');
        		old_nick = purple_connection_get_display_name(purple_account_get_connection(account));
			username = g_strsplit(purple_account_get_username(account), "@", 2);
			// if available default user, else concatenating the status
			if (purple_savedstatus_get_type(saved_status) == PURPLE_STATUS_AVAILABLE) {
				new_nick = g_strdup(username[0]);
			} else {
				new_nick = g_strconcat(username[0], separator, status_title, NULL);
			}
			if (g_strcmp0(old_nick, new_nick) != 0) {
				change_nick(account, new_nick);
			}
			// clean
			g_strfreev(username);
			g_free(status_title);
			g_free(new_nick);
		}
	}
}

static gboolean plugin_load(PurplePlugin *plugin) {

	PurplePlugin *irc;
	PurplePluginProtocolInfo *irc_info;
	PurpleAccountOption *option;
	void *savedstat_handle = purple_savedstatuses_get_handle();

	irc = purple_plugins_find_with_id(IRC_ID);
	if (NULL == irc)
		return FALSE;

	irc_info = PURPLE_PLUGIN_PROTOCOL_INFO(irc);
	if (NULL == irc_info)
		return FALSE;

	// assign the signal when saved-status changes
        purple_signal_connect(savedstat_handle, "savedstatus-changed", plugin,
		PURPLE_CALLBACK(status_changed_cb), NULL);
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	purple_debug(PURPLE_DEBUG_INFO, NAME, N_("plugin_unload called\n"));
	return TRUE;
}

static void init_plugin(PurplePlugin *plugin) {
	info.dependencies = g_list_append(info.dependencies, IRC_ID);
        bindtextdomain(PACKAGE, LOCALEDIR);

        info.name = _("IRC Status");
        info.summary = _("Changes your nick when changing your status in one IRC account.");
        info.description = _("This plugin changes your nick in the selected IRC account based on the piggin general status.");
        info.author = _("rickyepoderi");

	purple_prefs_add_none(IRCSTATUS_PREF_ROOT);
        purple_prefs_add_string(IRCSTATUS_PREF_SEPARATOR, DEFAULT_SEPARATOR);
        purple_prefs_add_string(IRCSTATUS_PREF_ACCOUNT, NULL);

}

PurplePluginPrefFrame *get_prefs_frame(PurplePlugin *plugin) {
	GList *accounts, *iter;
	PurpleAccount *account;
	PurplePluginPrefFrame *frame;
	PurplePluginPref *account_pref, *separator_pref;

	purple_debug(PURPLE_DEBUG_INFO, NAME, N_("creating preferences frame\n"));
	
	// a choice for the IRC account
	frame = purple_plugin_pref_frame_new();
	account_pref = purple_plugin_pref_new_with_name_and_label(IRCSTATUS_PREF_ACCOUNT, _("IRC account to update nick"));
        purple_plugin_pref_set_type(account_pref, PURPLE_PLUGIN_PREF_CHOICE);
	// read all accounts and display IRC ones
	accounts = purple_accounts_get_all();
	for (iter = accounts; iter; iter = iter->next) {
		account = iter->data;
		if (g_strcmp0(IRC_ID, purple_account_get_protocol_id(account)) == 0) {
			purple_plugin_pref_add_choice(account_pref, purple_account_get_username(account), (gpointer) purple_account_get_username(account));
		}
	}
	purple_plugin_pref_frame_add(frame, account_pref);

	// the separator character
	separator_pref = purple_plugin_pref_new_with_name_and_label(IRCSTATUS_PREF_SEPARATOR, _("Separator"));
	purple_plugin_pref_frame_add(frame, separator_pref);

	return frame;
}

PURPLE_INIT_PLUGIN(ircstatus, init_plugin, info);
