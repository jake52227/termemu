#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <sys/select.h>
#include "output_parser.hpp"

class Shell {
public:
	Shell();
	~Shell();
	void write_to(std::string msg);
	std::vector<struct Word> read_from();
private:
	int pty_primary;
	int pty_secondary;
	int shell_proc;
	fd_set rfds;
};

#endif
