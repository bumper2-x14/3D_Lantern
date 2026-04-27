#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <cstddef>

/// @brief Simple image container (stores pixel data).
class Image {
    public:
        int width; ///< Image width in pixels.
        int height; ///< Image height in pixels.
        int channels; ///< Number of channels (e.g. RGB = 3, RGBA = 4).
        unsigned char* data = nullptr; ///< Raw pixel data.

        // Constructors and destructor

        /// @brief Default constructor.
        Image() = default;

        /// @brief Destructor (frees image data).
        ~Image();

        // Delete copy semantics

        /// @brief Disable copy (to avoid double free / heavy copies).
        Image(const Image&) = delete;

        /// @brief Disable copy assignment.
        Image& operator=(const Image&) = delete;

        // Move semantics

        /// @brief Move constructor (transfers ownership of data).
        Image(Image&& other) noexcept;

        /// @brief Move assignment (transfers ownership of data).
        Image& operator=(Image&& other) noexcept;

        // Load and save

        /// @brief Loads an image from file.
        /// @param path Path to image file.
        static Image load(const std::string& path);

        /// @brief Saves the image as PNG.
        /// @param path Output file path.
        void savePNG(const std::string& path) const;

        /// @brief Returns pointer to pixel at (x, y).
        unsigned char* pixelData(int x, int y) const;

        // Helpers

        /// @brief Returns total size of image data in bytes.
        size_t size() const;
};

#endif