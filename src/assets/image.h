#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <cstddef>

class Image {
    public:
        int width;
        int height;
        int channels;
        unsigned char* data = nullptr;

        // Constructors and destructor
        Image() = default;
        ~Image();

        // Delete copy semantics
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        // Move semantics
        Image(Image&& other) noexcept;
        Image& operator=(Image&& other) noexcept;

        // Load and save
        static Image load(const std::string& path);
        void savePNG(const std::string& path) const;

        unsigned char* pixelData(int x, int y) const;

        // Helpers
        size_t size() const;
};

#endif
