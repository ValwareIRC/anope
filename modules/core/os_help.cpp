/* OperServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandOSHelp : public Command
{
 public:
	CommandOSHelp() : Command("HELP", 1, 1)
	{
	}

	CommandReturn Execute(CommandSource &source, const std::vector<Anope::string> &params)
	{
		mod_help_cmd(OperServ, source.u, params[0]);
		return MOD_CONT;
	}

	void OnSyntaxError(User *u, const Anope::string &subcommand)
	{
		u->SendMessage(OperServ, OPER_HELP);
		for (CommandMap::const_iterator it = OperServ->Commands.begin(), it_end = OperServ->Commands.end(); it != it_end; ++it)
			if (!Config->HidePrivilegedCommands || it->second->permission.empty() || (u->Account() && u->Account()->HasCommand(it->second->permission)))
				it->second->OnServHelp(u);
		u->SendMessage(OperServ, OPER_HELP_LOGGED);
	}
};

class OSHelp : public Module
{
	CommandOSHelp commandoshelp;

 public:
	OSHelp(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		this->AddCommand(OperServ, &commandoshelp);
	}
};

MODULE_INIT(OSHelp)
