#pragma once
#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>

namespace Utils
{
	class CommandParser
	{
	public:
		struct Options
		{
			char *programName;			// Duh
			bool fullscreen = false;	// Create window in fullscreen mode?
			struct Dimensions
			{
				int width;
				int height;
			} *dimensions = NULL;		// Window dimensions (if not fullscreen)
			bool showHelp = false;		// Show help and exit
		};

	private:
		static Options options;

	public:
		static const Options & getOptions() { return CommandParser::options; }
		// Parse command line arguments and apply them
		static void parse(int argc, char **argv);

	private:
		// Apply dimensions to the options
		// Dimensions should be in the form WxH
		static void parseDimensions(char **argv);
		// Creates a spaceship and sends it to
		// invade the USA ulimately failing and
		// not even bothering to look at the other
		// parts of the world
		static void showHelp();
		// Parse a long flag
		static void longFlag(char **argv);
		// Parse a short flag
		static void shortFlag(char **argv);
	};
}

#endif // !COMMAND_PARSER_H
