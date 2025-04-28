#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
namespace fs = filesystem;

void gaussianMask(const Mat &src, Mat &dst, int startRow, int endRow)
{
  int kernelSize = 7; // 7x7 blur (stronger)

  for (int r = startRow; r < endRow; r++)
  {
    for (int c = 0; c < src.cols; c++)
    {
      if (r < kernelSize / 2 || r >= src.rows - kernelSize / 2 || c < kernelSize / 2 || c >= src.cols - kernelSize / 2)
      {
        dst.at<Vec3b>(r, c) = src.at<Vec3b>(r, c);
        continue;
      }

      Vec3i sum(0, 0, 0);
      for (int kr = -kernelSize / 2; kr <= kernelSize / 2; kr++)
      {
        for (int kc = -kernelSize / 2; kc <= kernelSize / 2; kc++)
        {
          sum += src.at<Vec3b>(r + kr, c + kc);
        }
      }

      dst.at<Vec3b>(r, c) = sum / (kernelSize * kernelSize);
    }
  }
}

void processImages(const string &images, const string &blurred, int numThreads)
{
  // Create output folder if needed
  if (!fs::exists(blurred))
  {
    fs::create_directories(blurred);
    cout << "Created output folder: " << blurred << endl;
  }

  int imageCount = 0;
  double singleThreadTime = 0.0;
  double multiThreadTime = 0.0;

  // Check if input folder exists
  if (!fs::exists(images))
  {
    cout << "Error: Input folder doesn't exist: " << images << endl;
    return;
  }

  cout << "Looking for images in: " << fs::absolute(images) << endl;

  for (const auto &entry : fs::directory_iterator(images))
  {
    if (entry.is_regular_file())
    {
      string ext = entry.path().extension().string();
      for (auto &c : ext)
        c = tolower(c);

      if (ext != ".jpg" && ext != ".jpeg" && ext != ".png")
        continue;

      string filename = entry.path().filename().string();
      cout << "Processing: " << filename << endl;

      Mat img = imread(entry.path().string());
      if (img.empty())
      {
        cout << "  Failed to open image: " << entry.path() << endl;
        continue;
      }

      // SINGLE THREADED
      Mat singleResult = img.clone();
      auto start = chrono::high_resolution_clock::now();

      thread singleThread(gaussianMask, cref(img), ref(singleResult), 0, img.rows);
      singleThread.join();

      auto end = chrono::high_resolution_clock::now();
      double singleTime = chrono::duration<double, milli>(end - start).count();
      singleThreadTime += singleTime;

      string singlePath = blurred + "/single_" + filename;
      imwrite(singlePath, singleResult);

      // MULTI THREADED
      Mat multiResult = img.clone();
      start = chrono::high_resolution_clock::now();

      vector<thread> threads;
      int rowsPerThread = img.rows / numThreads;

      for (int i = 0; i < numThreads; i++)
      {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? img.rows : (i + 1) * rowsPerThread;

        threads.emplace_back(gaussianMask, cref(img), ref(multiResult), startRow, endRow);
      }

      for (auto &t : threads)
      {
        t.join();
      }

      end = chrono::high_resolution_clock::now();
      double multiTime = chrono::duration<double, milli>(end - start).count();
      multiThreadTime += multiTime;

      string multiPath = blurred + "/multi_" + filename;
      imwrite(multiPath, multiResult);

      // Print results
      cout << "  Single-thread: " << singleTime << " ms" << endl;
      cout << "  Multi-thread:  " << multiTime << " ms (" << numThreads << " threads)" << endl;
      cout << "  Speed-up:      " << singleTime / multiTime << "x" << endl;

      imageCount++;
    }
  }

  if (imageCount > 0)
  {
    cout << "\nProcessed " << imageCount << " images" << endl;
    cout << "Average single-thread time: " << singleThreadTime / imageCount << " ms" << endl;
    cout << "Average multi-thread time:  " << multiThreadTime / imageCount << " ms" << endl;
    cout << "Average speed-up:           " << singleThreadTime / multiThreadTime << "x" << endl;
  }
  else
  {
    cout << "No images found in " << images << endl;
    cout << "Make sure to add some .jpg or .png files to this directory." << endl;
  }
}

int main(int argc, char **argv)
{
  string images = "images";
  string blurred = "blurred";
  int numThreads = thread::hardware_concurrency();

  if (argc > 1)
    images = argv[1];
  if (argc > 2)
    blurred = argv[2];
  if (argc > 3)
    numThreads = stoi(argv[3]);

  cout << "Input folder: " << images << endl;
  cout << "Output folder: " << blurred << endl;
  cout << "Using " << numThreads << " threads for multi-threaded version" << endl;

  processImages(images, blurred, numThreads);

  return 0;
}
