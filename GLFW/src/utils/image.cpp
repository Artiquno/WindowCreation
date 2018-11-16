#include "image.h"

#include <stb_image.h>
#include <string>
#include <iostream>

namespace Utils
{
	Image::Image()
	{
		desiredChannel = 0;
	}
	Image::Image(const std::string& path, int desiredChannel)
	{
		setDesiredChannel(desiredChannel);
		setPath(path);
		load();
	}
	Image::~Image()
	{
		if (pixels)
		{
			stbi_image_free(pixels);
		}
	}

	void Image::load()
	{
		pixels = stbi_load_16(path.c_str(), &width, &height, &bpp, desiredChannel);
		
		size = width * height;
		if (!pixels)
		{
			std::cerr << "Could not load image " << path << std::endl;
		}
	}
	bool Image::isLoaded()
	{
		return pixels;
	}

	void Image::setDesiredChannel(int desiredChannel)
	{
		this->desiredChannel = desiredChannel;
	}
	void Image::setPath(const std::string& path)
	{
		this->path = path;
	}
	void Image::copyPixels(stbi_us *destination, size_t size)
	{
		memcpy(destination, pixels, size);
	}
	size_t Image::copyPixels(stbi_us *&destination)
	{
		destination = new stbi_us[size];
		memcpy(destination, pixels, size);
		return size;
	}
}