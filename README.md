Companion
================

Companion is an image recognition library written with OpenCV.

# Dependencies

* OpenCV Version 3.X
* OpenMP
* Cuda*

\* Optional

# Used Algorithms

* FeatureMatching (CPU + Cuda)

# Supported Features

* 2D Object Detection

# Planed Features & Algorithm

* Algorithm
  * Template Matching
  * Rectangle Detection
  * Tracking

* Features
  * 3D object Detection
  * Object Classification 
    * Bag of Words (HOG + SVM)
  
# Build

Simply use cmake or cmake-gui to build Companion for you platform. Enable the Cuda flag to build Companion with Cuda support.

```
cmake -DUSE_CUDA=ON
make
```

# Usage Examples

## Callback Handler

Example callback handler to obtain results.

```
void callback(std::vector<Drawable*> objects, cv::Mat frame) {
    Drawable *drawable;

    for(int x = 0; x < objects.size(); x++) {
        drawable = objects.at(x);
        drawable->draw(frame);
    }

    cv::imshow("Object detection", frame);
    cv::waitKey(1);
    frame.release();
}
```


## Error Handler

Example error handler to obtain results.

```
void error(CompanionError::errorCode code) {
    // Obtain detailed error message from code
    std::cout << CompanionError::getError(code) << "\n";
}
```

## Feature Matching

Create an empty Companion and configure it to detect 2D Objects.

```
Companion *companion = new Companion();

// Set video as input source to obtain images.
Stream *stream = new Video(videoPath);
companion->setSource(stream);

// Optional to skip frames
companion->setSkipFrame(2);

// Set callback handler
companion->setResultHandler(callback);
// Set an error callback handler
companion->setErrorHandler(error);

```

### Setup searched models

Setup feature matching models to detect for examples arts.

```
FeatureMatchingModel *object = new FeatureMatchingModel();
object->setImage(cv::imread(imagePath, cv::IMREAD_GRAYSCALE));

if(!companion->addModel(object)) {
    std::cout << "Model not added";
}
```

### CPU Usage

Sample to create an FeatureMatching with BRISK.

```
// Setup feature matching implementation from OpenCV
cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);
cv::Ptr<cv::BRISK> brisk = cv::BRISK::create(60);
ImageRecognition *recognition = new CPUFeatureMatching(brisk, brisk, matcher, type);
FeatureMatchingModel *object;
for (auto &image : images) {
    object = new FeatureMatchingModel();
    object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));
    if(!companion->addModel(object)) {
        std::cout << "Model not added";
    }
}
// Set image processing algorithm to your companion instance.
companion->setProcessing(new ObjectDetection(companion, recognition, 1));

// Create an stream worker class
StreamWorker ps(queue);
try {
    // Execute companion
    companion->run(ps);
} catch (CompanionError::errorCode errorCode) {
    error(errorCode);
}
```

### Cuda Usage

For Cuda only ORB and Surf are implemented in OpenCV.

```
// Setup feature matching implementation from OpenCV
cv::Ptr<cv::cuda::ORB> GPU_ORB = cv::cuda::ORB::create(6000);
GPU_ORB->setBlurForDescriptor(true);
ImageRecognition *recognition = new CudaFeatureMatching(GPU_ORB);

// Set image processing algorithm to your companion instance.
companion->setProcessing(new ObjectDetection(companion, recognition, 1));

// Create an stream worker class
StreamWorker ps(queue);
try {
    // Execute companion
    companion->run(ps);
} catch (CompanionError::errorCode errorCode) {
    error(errorCode);
}
```

### License

This program is an image recognition library written with OpenCV.
Copyright (C) 2016-2017 Andreas Sekulski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
