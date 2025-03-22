#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>  
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Function to convert sRGB to linear RGB
void rgb_to_linear(double r_srgb, double g_srgb, double b_srgb, double *r_linear, double *g_linear, double *b_linear) {
    *r_linear = (r_srgb <= 0.04045) ? (r_srgb / 12.92) : pow((r_srgb + 0.055) / 1.055, 2.4);
    *g_linear = (g_srgb <= 0.04045) ? (g_srgb / 12.92) : pow((g_srgb + 0.055) / 1.055, 2.4);
    *b_linear = (b_srgb <= 0.04045) ? (b_srgb / 12.92) : pow((b_srgb + 0.055) / 1.055, 2.4);
}

// Function to calculate luminance from linear RGB values
double calculate_luminance(double r_linear, double g_linear, double b_linear) {
    return 0.2126 * r_linear + 0.7152 * g_linear + 0.0722 * b_linear;
}

// Function to convert linear luminance back to sRGB
double gamma_compress(double luminance) {
    return (luminance <= 0.0031308) ? (luminance * 12.92) : (1.055 * pow(luminance, 1 / 2.4) - 0.055);
}

// Function to convert grayscale value to ASCII character
char grayscale_to_ascii(unsigned char gray) {
    const char* ascii_chars = "@%#*+=-:. "; // Characters from dark to light
    int index = gray * (strlen(ascii_chars) - 1) / 255; // Map gray value to index
    return ascii_chars[index];
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    // Get the image file path from command-line argument
    const char *image_path = argv[1];

    int width, height, channels;

    // Load the image
    unsigned char *image = stbi_load(image_path, &width, &height, &channels, 0);
    if (image == NULL) {
        printf("Error loading image: %s\n", image_path);
        return 1;
    }

    printf("Image loaded: %dx%d with %d channels\n", width, height, channels);

    // Create an array to store the grayscale image data
    unsigned char *grayscale_image = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (grayscale_image == NULL) {
        printf("Error allocating memory for grayscale image\n");
        stbi_image_free(image);
        return 1;
    }

    // Process each pixel to convert to grayscale
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels; // Get the index of the pixel

            unsigned char r = image[i];      // Red
            unsigned char g = image[i + 1];  // Green
            unsigned char b = image[i + 2];  // Blue

            // Linearize the RGB values
            double r_linear, g_linear, b_linear;
            rgb_to_linear(r / 255.0, g / 255.0, b / 255.0, &r_linear, &g_linear, &b_linear);

            // Calculate the linear luminance (grayscale value)
            double luminance = calculate_luminance(r_linear, g_linear, b_linear);

            // Gamma-compress the luminance back to sRGB and store it as grayscale
            unsigned char grayscale = (unsigned char)(gamma_compress(luminance) * 255.0);

            // Store the grayscale value in the new image
            grayscale_image[y * width + x] = grayscale;
        }
    }

    // Generate ASCII art from the grayscale image
    FILE *ascii_file = fopen("ascii_art.txt", "w");
    if (!ascii_file) {
        printf("Error opening file for ASCII art output\n");
        free(grayscale_image);
        stbi_image_free(image);
        return 1;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char gray_value = grayscale_image[y * width + x];
            char ascii_char = grayscale_to_ascii(gray_value);
            fprintf(ascii_file, "%c", ascii_char);
        }
        fprintf(ascii_file, "\n"); // New line at the end of each row
    }

    fclose(ascii_file);
    printf("ASCII art saved successfully as 'ascii_art.txt'\n");

    // Free memory
    free(grayscale_image);
    stbi_image_free(image);

    return 0;
}
