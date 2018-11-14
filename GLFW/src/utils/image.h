#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <stb_image.h>

namespace Utils
{
	class Image
	{
	private:
		std::string path;
		int width;
		int height;
		int bpp;
		int desiredChannel;
		size_t size;
		stbi_uc *pixels;

	public:
		Image();
		// Load from path
		Image(const std::string& path, int desiredChannel = 0);
		~Image();

		void load();
		bool isLoaded();

		size_t getSize() const { return size; }
		int getHeight() const { return height; }
		int getWidth() const { return width; }
		int getBpp() const { return bpp; }
		// Probably unsafe, use this until i find a better way
		stbi_uc * const getPixels() const { return pixels; }
		const std::string& getPath() const { return path; }

		void setDesiredChannel(int desiredChannel);
		void setPath(const std::string& path);

		// Copy pixels to destination
		// destination must be allocated
		void copyPixels(stbi_uc *destination, size_t size);
		// Allocate memory for destination and copy pixels
		size_t copyPixels(stbi_uc *&destination);
	};
}

#endif // !IMAGE_H
