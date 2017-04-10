#include "CudaFeatureMatching.h"

CudaFeatureMatching::CudaFeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching) {
    this->cudaFeatureMatching = cudaFeatureMatching;

}

CudaFeatureMatching::~CudaFeatureMatching() {

}

Drawable* CudaFeatureMatching::algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel) {

    Drawable *lines = nullptr;
    int ngpus = cv::cuda::getCudaEnabledDeviceCount();

    cv::Mat img_matches;
    cv::Mat scene_rgb = searchModel->getImage();
    cv::Mat object = compareModel->getImage();
    cv::Mat scene;
    std::vector<cv::KeyPoint> keypoints_scene, keypoints_object;

    // ToDo := Check if Image is in correct format
    cvtColor(scene_rgb, scene, CV_RGB2GRAY);

    FeatureMatchingModel *sModel = dynamic_cast<FeatureMatchingModel *>(searchModel);
    FeatureMatchingModel *cModel = dynamic_cast<FeatureMatchingModel *>(compareModel);

    if(ngpus > 0) {

        // ---- Cuda start
        cv::cuda::GpuMat gpu_scene(scene);
        cv::cuda::GpuMat gpu_object(object);

        cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;

        cudaFeatureMatching->detectAndCompute(gpu_scene, cv::noArray(), keypoints_scene, gpu_descriptors_scene);
        cudaFeatureMatching->detectAndCompute(gpu_object, cv::noArray(), keypoints_object, gpu_descriptors_object);

        cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cudaFeatureMatching->defaultNorm());
        std::vector<std::vector<cv::DMatch>> matches;
        gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);

        gpu_scene.release();
        gpu_object.release();
        // ---- Cuda end

        std::vector<cv::DMatch> good_matches;
        ratio_test(matches, good_matches, 0.80);

        lines = obtainMatchingResult(scene, object, good_matches, keypoints_object, keypoints_scene, sModel, cModel);
    }

    return lines;
}

bool CudaFeatureMatching::isCuda() {
    return true;
}