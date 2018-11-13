#include "command_parser.h"

#include <iostream>

#include "../definitions.h"

namespace Utils
{
	CommandParser::Options CommandParser::options;
	void CommandParser::showHelp()
	{
		std::cout << "Hi! I'm helping!" << std::endl;
	}

	void CommandParser::longFlag(char** argv)
	{
		std::cerr << "Long flags are not yet implemented." << std::endl;
	}
	void CommandParser::shortFlag(char** argv)
	{
		char *flag = &(*argv)[1];
		while (*flag != '\0')
		{
			switch (*flag)
			{
			case 'f':	options.fullscreen = true; break;
			// Assumes the next argument is the dimensions
			case 'd':	parseDimensions(++argv); break;
			case 'h':
			case '?':
				showHelp();
				exit(ExitStatus::Normal);
				break;
			default:
				std::cerr << "Flag " << flag << " is not a valid flag" << std::endl;
				showHelp();
				exit(ExitStatus::InvalidFlag);
			}
			++flag;	// Move to the next character
		}
	}

	void CommandParser::parse(int argc, char **argv)
	{
		options.programName = argv++[0];	// Don't know why I need this but it doesn't hurt?
		while (argv[0] != NULL)
		{
			if (argv[0][0] == '-')
			{
				if (argv[0][1] == '-')
				{
					longFlag(argv);
				}
				else
				{
					shortFlag(argv);
				}
			}
			++argv;	// Move to the next argument
		}

		// If no dimensions are provided set window to fullscreen
		if (options.dimensions == NULL)
		{
			options.fullscreen = true;
		}
	}

	void CommandParser::parseDimensions(char **argv)
	{
		if (options.dimensions != NULL)
		{
			std::cerr << "Window dimensions already set to " << options.dimensions->width << "x" << options.dimensions->height << std::endl;
			return;
		}
		// Probably bad way to do it but it's simple and it works?
		char** next = new char*[1];
		const char *dims = strtok_s(*argv, "x", next);
		options.dimensions = new Options::Dimensions;
		options.dimensions->width = (int)strtol(dims, NULL, 10);
		options.dimensions->height = (int)strtol(*next, NULL, 10);
	}
}