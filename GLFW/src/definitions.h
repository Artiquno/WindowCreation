#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

enum ExitStatus
{
	Unknown = -1,
	Normal,
	InvalidFlag,
	GlfwError,
	VertexShaderError,
	FragmentShaderError,
	ShaderProgramLinkError,
	WindowCreationError,
	FileOpenError,
	FileReadError,
	FileWriteError
};

#endif // !DEFINITIONS_H
