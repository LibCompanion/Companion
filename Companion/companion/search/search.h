#pragma once

#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

/**
 * @brief Abstract search class for specific search models.
 * @author Andreas Sekulski
 */
class Search
{
public:

	/**
	 * @brief show_images Visualize search image and compare image image if exists in an window to compare.
	 */
	void show_images();

	/**
	 * @brief Sets the search image path.
	 *
	 * @param search_image_path  The search image path.
	 */
	void set_search_image_path(string search_image_path);

	/**
	 * @brief Gets the search image path.
	 *
	 * @return The search image path.
	 */
	string get_search_image_path();

	/**
	 * @brief Sets the compare image path.
	 *
	 * @param compare_image_path  The compare image path.
	 */
	void set_compare_image_path(string compare_image_path);

	/**
	 * @brief Gets the compare image path.
	 *
	 * @return The compare image path.
	 */
	string get_compare_image_path();

	/**
	 * @brief Sets the accordance.
	 *
	 * @param accordance The accordance to set.
	 */
	void set_accordance(double accordance);

	/**
	 * @brief Gets the accordance from search.
	 *
	 * @return The accordance from search.
	 */
	double get_accordance();

protected:

	string search_image_path;  ///< Image path from search.
	                          
	string compare_image_path; ///< Image path from search.

	double accordance; ///< Accordance from search between search and compare image.
};

