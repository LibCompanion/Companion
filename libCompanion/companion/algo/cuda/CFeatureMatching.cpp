#include "CFeatureMatching.h"

CFeatureMatching::CFeatureMatching() {

}

CFeatureMatching::~CFeatureMatching() {

}

Drawable* CFeatureMatching::algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel) {

    Drawable *lines = nullptr;
    int ngpus = cv::cuda::getCudaEnabledDeviceCount();

    cv::Mat img_matches;
    cv::Mat scene_rgb = searchModel->getImage();
    cv::Mat object = compareModel->getImage();
    cv::Mat scene;
    std::vector<cv::KeyPoint> keypoints_scene, keypoints_object;

    cvtColor(scene_rgb, scene, CV_RGB2GRAY);

    FeatureMatchingModel *sModel = dynamic_cast<FeatureMatchingModel *>(searchModel);
    FeatureMatchingModel *cModel = dynamic_cast<FeatureMatchingModel *>(compareModel);

    cModel->setLastPosition(-1, -1, 0, 0);

    // If wrong model types are used...
    if(!sModel || !cModel) {
        throw CompanionError::errorCode::wrong_model_type;
    }

    if(ngpus > 0) {

        // ---- Cuda start
        cv::cuda::GpuMat gpu_scene(scene);
        cv::cuda::GpuMat gpu_object(object);
        cv::Ptr<cv::cuda::ORB> orb = cv::cuda::ORB::create(6000);
        orb->setBlurForDescriptor(true);

        cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;

        orb->detectAndCompute(gpu_scene, cv::noArray(), keypoints_scene, gpu_descriptors_scene);
        orb->detectAndCompute(gpu_object, cv::noArray(), keypoints_object, gpu_descriptors_object);

        cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(orb->defaultNorm());
        std::vector<std::vector<cv::DMatch>> matches;
        gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);

        std::vector<cv::DMatch> good_matches;
        ratio_test(matches, good_matches, 0.80);
        // ---- Cuda end

        lines = obtainMatchingResult(scene, object, good_matches, keypoints_object, keypoints_scene, sModel, cModel);
        if(lines == nullptr) {
            // If result is not good enough and IRA was used.
            if(compareModel->isLastPositionSet()) {
                compareModel->setLastPosition(-1, -1, 0, 0); // Reset position because object is no more detected...
                return algo(searchModel, compareModel);
            }
        }

    }


    return lines;
}