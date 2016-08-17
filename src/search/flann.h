#pragma once

#include "search.h"
#include <iostream>

using namespace std;

/**
 * @brief Fast library for approximate nearest neighbors match model search class for an flann search.
 * @author Andreas Sekulski
 */
class Flann : public Search
{
public:

	/**
	 * @brief Constructor to create an empty flann model for an flann search.
	 */
	Flann();

	/**
	 * @brief Constructor to create an flann match model for an flann search.
	 *
	 * @param search_image_path   The search image path from search.
	 * @param compare_image_path  The compare image path from search.
	 * @param accordance          The accordance value from search.
	 * @param matches_size        The match poitns which match from search.
	 */
	Flann(string search_image_path, string compare_image_path, double accordance, int matches_size);

	/**
	 * @brief Destroys the object.
	 */
	~Flann();

	/**
	 * @brief Sets match points from search.
	 *
	 * @param matches_size  Match points which match.
	 */
	void set_matches_size(int matches_size);

	/**
	 * @brief Gets the match points from search.
	 *
	 * @return All match points from search.
	 */
	int get_matches_size();

private:

	int matches_size; ///< Counter from match points from search between search and compare image.
};
