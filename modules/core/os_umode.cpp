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

class CommandOSUMode : public Command
{
 public:
	CommandOSUMode() : Command("UMODE", 2, 2, "operserv/umode")
	{
	}

	CommandReturn Execute(CommandSource &source, const std::vector<Anope::string> &params)
	{
		User *u = source.u;
		const Anope::string &nick = params[0];
		const Anope::string &modes = params[1];

		User *u2;

		/**
		 * Only accept a +/- mode string
		 *-rob
		 **/
		if (modes[0] != '+' && modes[0] != '-')
		{
			this->OnSyntaxError(u, "");
			return MOD_CONT;
		}
		if (!(u2 = finduser(nick)))
			source.Reply(NICK_X_NOT_IN_USE, nick.c_str());
		else
		{
			u2->SetModes(OperServ, "%s", modes.c_str());

			source.Reply(OPER_UMODE_SUCCESS, nick.c_str());
			u2->SendMessage(OperServ, OPER_UMODE_CHANGED, u->nick.c_str());

			if (Config->WallOSMode)
				ircdproto->SendGlobops(OperServ, "\2%s\2 used UMODE on %s", u->nick.c_str(), nick.c_str());
		}
		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &subcommand)
	{
		u->SendMessage(OperServ, OPER_HELP_UMODE);
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &subcommand)
	{
		SyntaxError(OperServ, u, "UMODE", OPER_UMODE_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(OperServ, OPER_HELP_CMD_UMODE);
	}
};

class OSUMode : public Module
{
	CommandOSUMode commandosumode;

 public:
	OSUMode(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		if (!ircd->umode)
			throw ModuleException("Your IRCd does not support setting umodes");

		this->SetAuthor("Anope");
		this->SetType(CORE);

		this->AddCommand(OperServ, &commandosumode);
	}
};

MODULE_INIT(OSUMode)
