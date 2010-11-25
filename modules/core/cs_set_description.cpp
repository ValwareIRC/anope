/* ChanServ core functions
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

class CommandCSSetDescription : public Command
{
 public:
	CommandCSSetDescription(const Anope::string &cpermission = "") : Command("DESC", 2, 2, cpermission)
	{
	}

	CommandReturn Execute(CommandSource &source, const std::vector<Anope::string> &params)
	{
		User *u = source.u;
		ChannelInfo *ci = source.ci;
		if (!ci)
			throw CoreException("NULL ci in CommandCSSetDescription");

		ci->desc = params[1];

		u->SendMessage(ChanServ, CHAN_DESC_CHANGED, ci->name.c_str(), ci->desc.c_str());

		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &)
	{
		u->SendMessage(ChanServ, CHAN_HELP_SET_DESC, "SET");
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &)
	{
		// XXX
		SyntaxError(ChanServ, u, "SET", CHAN_SET_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(ChanServ, CHAN_HELP_CMD_SET_DESC);
	}
};

class CommandCSSASetDescription : public CommandCSSetDescription
{
 public:
	CommandCSSASetDescription() : CommandCSSetDescription("chanserv/saset/description")
	{
	}

	bool OnHelp(User *u, const Anope::string &)
	{
		u->SendMessage(ChanServ, CHAN_HELP_SET_DESC, "SASET");
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &)
	{
		// XXX
		SyntaxError(ChanServ, u, "SASET", CHAN_SASET_SYNTAX);
	}
};

class CSSetDescription : public Module
{
	CommandCSSetDescription commandcssetdescription;
	CommandCSSASetDescription commandcssasetdescription;

 public:
	CSSetDescription(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		Command *c = FindCommand(ChanServ, "SET");
		if (c)
			c->AddSubcommand(&commandcssetdescription);

		c = FindCommand(ChanServ, "SASET");
		if (c)
			c->AddSubcommand(&commandcssasetdescription);
	}

	~CSSetDescription()
	{
		Command *c = FindCommand(ChanServ, "SET");
		if (c)
			c->DelSubcommand(&commandcssetdescription);

		c = FindCommand(ChanServ, "SASET");
		if (c)
			c->DelSubcommand(&commandcssasetdescription);
	}
};

MODULE_INIT(CSSetDescription)
