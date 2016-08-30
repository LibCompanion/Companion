#pragma once

#include "search.h"
#include <iostream>
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

/**
 * @brief FeatureMatch match model search class for an feature search.
 * @author Andreas Sekulski
 */
class FeatureMatch : public Search
{
public:

	/**
	 * @brief Constructor to create an empty feature model.
	 */
	FeatureMatch();

	/**
	 * @brief Constructor to create an feature match model.
	 *
	 * @param search_image_path   The search image path from search.
	 * @param compare_image_path  The compare image path from search.
	 * @param accordance          The accordance value from search.
	 * @param matches             The match poitns which match from search.
	 */
	FeatureMatch(string search_image_path, string compare_image_path, double accordance, vector<DMatch> matches, vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2);

	/**
	 * @brief Destroys the object.
	 */
	~FeatureMatch();

	/**
	 * @brief Sets match points from search.
	 *
	 * @param matches Match points which match.
	 */
	void set_matches(vector<DMatch> matches);

	/**
	 * @brief Gets the match points from search.
	 *
	 * @return All match points from search.
	 */
	vector<DMatch> get_matches();

	/**
	* @brief show_compare_points Shows given result to an window to compare if match points are correctly.
	*/
	void show_compare_points();

private:

	vector<DMatch> matches; ///< Match points from search between search and compare image.

	vector<KeyPoint> keypoints_1;

	vector<KeyPoint> keypoints_2;
};
