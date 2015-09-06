/*
 *
 * Copyright (C) 2008-2011 Robin Burchell <w00t@inspircd.org>
 * Copyright (C) 2008-2014 Anope Team <team@anope.org>
 *
 * Please read COPYING and README for further details.
 *
 */

#pragma once

#include "users.h"
#include "anope.h"
#include "serialize.h"
#include "commands.h"
#include "config.h"

/* A service bot (NickServ, ChanServ, a BotServ bot, etc). */
class CoreExport ServiceBot : public LocalUser
{
 public:
	/* Underlying botinfo for this bot */
	Serialize::Reference<BotInfo> bi;
	/* Map of actual command names -> service name/permission required */
	CommandInfo::map commands;
	/* Modes the bot should have as configured in service:modes */
	Anope::string botmodes;
	/* Whether or not this bot is introduced to the network */
	bool introduced;

	/** Create a new bot.
	 * @param nick The nickname to assign to the bot.
	 * @param user The ident to give the bot.
	 * @param host The hostname to give the bot.
	 * @param real The realname to give the bot.
	 * @param bmodes The modes to give the bot.
	 */
	ServiceBot(const Anope::string &nick, const Anope::string &user = "", const Anope::string &host = "", const Anope::string &real = "", const Anope::string &bmodes = "");

	/** Destroy a bot, clearing up appropriately.
	 */
	virtual ~ServiceBot();

	void GenerateUID();

	void OnKill();

	/** Change the nickname for the bot.
	 * @param newnick The nick to change to
	 */
	void SetNewNick(const Anope::string &newnick);

	/** Return the channels this bot is assigned to
	 */
	std::vector<ChanServ::Channel *> GetChannels() const;

	/** Assign this bot to a given channel, removing the existing assigned bot if one exists.
	 * @param u The user assigning the bot, or NULL
	 * @param ci The channel registration to assign the bot to.
	 */
	void Assign(User *u, ChanServ::Channel *ci);

	/** Remove this bot from a given channel.
	 * @param u The user requesting the unassign, or NULL.
	 * @param ci The channel registration to remove the bot from.
	 */
	void UnAssign(User *u, ChanServ::Channel *ci);

	/** Get the number of channels this bot is assigned to
	 */
	unsigned GetChannelCount() const;

	/** Join this bot to a channel
	 * @param c The channel
	 * @param status The status the bot should have on the channel
	 */
	void Join(Channel *c, ChannelStatus *status = NULL);

	/** Join this bot to a channel
	 * @param chname The channel name
	 * @param status The status the bot should have on the channel
	 */
	void Join(const Anope::string &chname, ChannelStatus *status = NULL);

	/** Part this bot from a channel
	 * @param c The channel
	 * @param reason The reason we're parting
	 */
	void Part(Channel *c, const Anope::string &reason = "");

	/** Called when a user messages this bot
	 * @param u The user
	 * @param message The users' message
	 */
	virtual void OnMessage(User *u, const Anope::string &message);

	/** Link a command name to a command in services
	 * @param cname The command name
	 * @param sname The service name
	 * @param permission Permission required to execute the command, if any
	 * @return The commandinfo for the newly created command
	 */
	CommandInfo& SetCommand(const Anope::string &cname, const Anope::string &sname, const Anope::string &permission = "");

	/** Get command info for a command
	 * @param cname The command name
	 * @return A struct containing service name and permission
	 */
	CommandInfo *GetCommand(const Anope::string &cname);

	CommandInfo *FindCommand(const Anope::string &service);

	/** Find a bot by nick
	 * @param nick The nick
	 * @param nick_only True to only look by nick, and not by UID
	 * @return The bot, if it exists
	 */
	static ServiceBot* Find(const Anope::string &nick, bool nick_only = false);
};

class BotInfo : public Serialize::Object
{
 public:
	ServiceBot *bot;
	Configuration::Block *conf = nullptr;

	BotInfo(Serialize::TypeBase *type) : Serialize::Object(type) { }
	BotInfo(Serialize::TypeBase *type, Serialize::ID id) : Serialize::Object(type, id) { }

	void Delete() override;

	void SetNick(const Anope::string &);
	Anope::string GetNick();

	void SetUser(const Anope::string &);
	Anope::string GetUser();

	void SetHost(const Anope::string &);
	Anope::string GetHost();

	void SetRealName(const Anope::string &);
	Anope::string GetRealName();

	void SetCreated(const time_t &);
	time_t GetCreated();

	void SetOperOnly(const bool &);
	bool GetOperOnly();
};

class BotInfoType : public Serialize::Type<BotInfo>
{
 public:
	Serialize::Field<BotInfo, Anope::string> nick, user, host, realname;
	Serialize::Field<BotInfo, time_t> created;
	Serialize::Field<BotInfo, bool> operonly;

	BotInfoType() : Serialize::Type<BotInfo>(nullptr, "BotInfo")
		, nick(this, "nick")
		, user(this, "user")
		, host(this, "host")
		, realname(this, "realname")
		, created(this, "created")
		, operonly(this, "operonly")
	{
	}
};

static Serialize::TypeReference<BotInfo> botinfo("BotInfo");

