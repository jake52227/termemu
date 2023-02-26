#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <sys/select.h>
#include <vector>
#include <memory>
#include "parser.hpp"

class Shell
{
public:
	static std::unique_ptr<Shell> create();
	~Shell();
	void write_to(std::string msg);
	std::string read_from();
private:
	int pty_primary;
	int pty_secondary;
	int shell_proc;
	fd_set rfds;
};

#endif
