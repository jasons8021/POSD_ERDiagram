/*
 * History.h
 *
 *  Created on: 2009/11/29
 *      Author: zwshen
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <gtest/gtest_prod.h> 
#include "Command.h"
#include <stack>

using namespace std;

class CommandManager {
// 	friend class CommandManagerTest;
// 	FRIEND_TEST(CommandManagerTest, execute);
// 	FRIEND_TEST(CommandManagerTest, redo);
// 	FRIEND_TEST(CommandManagerTest, undo);
public:
	CommandManager();
	virtual ~CommandManager();
	void execute(Command* c);
	bool redo();
	bool undo();
	int getUndoCmdsSize();
	int getRedoCmdsSize();
private:
	stack<Command*> _undoCmds;
	stack<Command*> _redoCmds;
};

#endif 
