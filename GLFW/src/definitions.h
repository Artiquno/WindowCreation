#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

enum ExitStatus
{
	Unknown = -1,
	Normal,
	InvalidFlag,
	GlfwError,
	ShaderCompileError,
	ShaderProgramLinkError,
	WindowCreationError,
	FileOpenError,
	FileReadError,
	FileWriteError
};

#endif // !DEFINITIONS_H
