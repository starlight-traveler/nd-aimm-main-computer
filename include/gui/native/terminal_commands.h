#ifndef TERMINAL_COMMANDS_H
#define TERMINAL_COMMANDS_H

#include <array>
#include <string>
#include <vector>
#include <mutex>

#include "terminal.h"
#include "terminal_helpers.h"

struct custom_command_struct {
	bool should_close = false;
};

class terminal_commands : public ImTerm::basic_spdlog_terminal_helper<terminal_commands, custom_command_struct, misc::no_mutex> {
public:

	terminal_commands();

	static std::vector<std::string> no_completion(argument_type&) { return {}; }

	static void clear(argument_type&);
	static void configure_term(argument_type&);
	static std::vector<std::string> configure_term_autocomplete(argument_type&);
	static void echo(argument_type&);
	static void exit(argument_type&);
	static void help(argument_type&);
	static void quit(argument_type&);
};

#endif TERMINAL_COMMANDS_H