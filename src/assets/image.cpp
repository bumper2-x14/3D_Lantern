#include <string>
#include <stdexcept>

#include "image.h"
#include "stb_image.h"

// Constructors and destructor
Image::~Image() {
    if (data)
        stbi_image_free(data);
}


// Move semantics
Image::Image(Image&& other) noexcept
        : width(other.width), height(other.height),
          channels(other.channels), data(other.data) {
        other.data = nullptr;
        other.width = other.height = other.channels = 0;
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
            if (data) stbi_image_free(data);
            data = other.data;
            width = other.width;
            height = other.height;
            channels = other.channels;

            other.data = nullptr;
            other.width = other.height = other.channels = 0;
    }
    return *this;
}

// Load and save
Image Image::load(const std::string& path) {
    Image img;
    img.data = stbi_load(path.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data) {
        throw std::runtime_error("Failed to load image: " + path);
    }
    return img;
}

unsigned char* Image::pixelData(int x, int y) const {
    if (!data) return nullptr;
    if (x < 0) x = 0;
    if (x >= width) x = width - 1;
    if (y < 0) y = 0;
    if (y >= height) y = height - 1;
    return data + (y * width + x) * channels;
}

//void savePNG(const std::string& path) const;

size_t Image::size() const {
    return width * height * channels;
}
