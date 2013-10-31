#include "CommandManager.h"

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
	while (!_undoCmds.empty()) {
		Command* cmd = _undoCmds.top();
		_undoCmds.pop();
		delete cmd;
	}
	while (!_redoCmds.empty()) {
		Command* cmd = _redoCmds.top();
		_redoCmds.pop();
		delete cmd;
	}
}

void CommandManager::execute(Command* cmd)
{
	cmd->execute();
	_undoCmds.push(cmd);

	// cleanup and release redoable commands
	while (!_redoCmds.empty()) {
		Command* command = _redoCmds.top();
		_redoCmds.pop();
		delete command;
	}
}

bool CommandManager::redo()
{
	if (_redoCmds.size() == 0)
		return false; // or throw exception

	Command* cmd = _redoCmds.top();
	_redoCmds.pop();
	cmd->execute(); // redo the command
	_undoCmds.push(cmd);

	return true;
}

bool CommandManager::undo()
{
	if (_undoCmds.size() == 0)
		return false;

	Command* cmd = _undoCmds.top();
	_undoCmds.pop();
	cmd->unexecute(); // undo the command
	_redoCmds.push(cmd);

	return true;
}
