# Gaussian Blur Image Processing with Multi-threading

## Overview

This project demonstrates how to apply a **Gaussian Blur** to a set of images in both single-threaded and multi-threaded modes, and measures the performance difference between these two approaches. The project uses **C++** and **OpenCV** for image processing and **C++ thread library** for multi-threading control.

## Requirements

- **C++ Compiler**: Supports C++17 or later
- **OpenCV**: Version 4.x or later
- **CMake**: Version 3.10 or later

## Use Cases

1. **Single-Threaded Performance**:

   - Applies a **7x7 Gaussian Blur** to each image in a **single thread**.
   - Useful to benchmark the performance when the image processing is done on a single CPU core.
   - Measures the time taken to process the entire image.

2. **Multi-Threaded Performance**:
   - Applies the **same 7x7 Gaussian Blur**, but divides the image processing across **multiple threads** (based on the number of CPU cores).
   - Measures the time taken to process the entire image using multiple threads.
   - Compares the performance gain from parallelizing the image processing.

## Libraries Used

- **OpenCV**:
  - Used for image loading, saving, and matrix operations such as applying the blur. It handles various image formats such as `.jpg`, `.png`, and `.jpeg`.
  - `imread()`: Reads an image from the disk.
  - `imwrite()`: Writes the processed image to disk.
  - `cv::Mat`: Represents the image as a matrix for efficient pixel access.
- **C++ Standard Library (Thread)**:

  - Used for **multi-threading** the image processing. The image is divided into regions, and each region is processed in parallel across different threads.
  - `std::thread`: Used to create threads for parallel image processing.
  - `std::chrono`: Measures the time taken by single-threaded and multi-threaded executions.

- **C++ Standard Library (Filesystem)**:
  - Used for managing directories and files on the system.
  - `std::filesystem::create_directories()`: Creates the output folder if it doesn't exist.
  - `std::filesystem::directory_iterator`: Iterates over the images in the input folder.

## Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/TarunZach/opencv_imageblur.git
```

### 2. Building on MacOS

- Install Dependencies

1. Install Homebrew if you don't have it yet

   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

   # Install OpenCV and CMake
   brew install opencv
   brew install cmake
   ```

   2. Create and Build the Project

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. Run the Project
   ```bash
   ./Folder_Name ../images ../blurred
   ```

- images: Path to the input image folder containing .jpg, .jpeg, or .png files.
- blurred: Path to the output folder where the processed images will be saved.
- Optionally you can also specify the number of threads to be used:

```bash
./MCP_Assignment1 ../images ../blurred 4
```

### 3. Building on Windows

1. Install Dependencies

   You need to install OpenCV and CMake on Windows:

   CMake: https://cmake.org/download/

   Install OpenCV using Pre-built OpenCV binaries for Windows.

   OpenCV: https://opencv.org/releases/

2. Create and Build the Project

- Open CMake GUI.
- Set the source folder to your project directory.
- Set the build folder to build (create it).
- Click Configure, then Generate.
- Open the generated MCP_Assignment1.sln in Visual Studio.
- Build the project using Visual Studio.

3. Run the Project

   Open the Command Prompt and navigate to the build directory:

   ```bash
   MCP_Assignment1.exe ..\images ..\blurred
   ```

## Performance Metrics

The program measures:

- Single-threaded execution time: Time taken to process each image with a single thread.
- Multi-threaded execution time: Time taken to process each image with the specified number of threads.
- Speed-up: The ratio of single-threaded time to multi-threaded time. A speed-up greater than 1 indicates better performance with multi-threading.

## Example Output

```bash
Processing: image1.jpg
Single-thread: 500 ms
Multi-thread: 200 ms (4 threads)
Speed-up: 2.5x
```

## Troubleshooting

### Images Not Found

Ensure the images are in the correct input folder and that the filenames have the correct extension (.jpg, .jpeg, .png).

### Build Issues

If you encounter build errors, make sure you have the correct OpenCV version and CMake version installed. If necessary, check the OpenCV installation paths and ensure that the project is linked to OpenCV correctly.
