#pragma once

#include "search.h"
#include <iostream>

using namespace std;

/**
 * @brief Template match model search class for an template match search.
 * @author Andreas Sekulski
 */
class TemplateMatch : public Search
{
public:

	/**
	 * @brief Constructor to create an empty template match model for an template match search.
	 */
	TemplateMatch();

	/**
	 * @brief Constructor to create an template match model for an template match search.
	 *
	 * @param search_image_path   The search image path from search.
	 * @param compare_image_path  The compare image path from search.
	 * @param accordance          The accordance value from search.
	 * @param match_point         The matching point coordinates.
	 * @param match_offset        The offset to draw rectangle to his correclty position.
	 * @param result_matching     The result image from template operation.
	 */
	TemplateMatch(string search_image_path, string compare_image_path, double accordance, Point match_point, Point match_offset);

	/**
	 * @brief Destroys the object.
	 */
	~TemplateMatch();

	/**
	 * @brief show_compare_points Shows given result to an window to compare if match points are correctly.
	 */
	void show_compare_points();

	/* ToDo Getter and Setter */

private:

	/*ToDo Documentation*/
	Point match_point;

	/*ToDo Documentation*/
	Point match_offset;
};

