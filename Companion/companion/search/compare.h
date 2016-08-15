#pragma once

#include "search.h"
#include <iostream>

using namespace std;

/**
 * @brief Compare model search class for an simple compare search.
 * @author Andreas Sekulski
 */
class Compare : public Search
{
public:

	/**
	 * @brief Constructor to create an empty compare model for an simple compare search.
	 */
	Compare();

	/**
	 * @brief Constructor to create an compare model for an simple compare search.
	 *
	 * @param search_image_path   The search image path from search.
	 * @param compare_image_path  The compare image path from search.
	 * @param accordance          The accordance value from search.
	 */
	Compare(string search_image_path, string compare_image_path, double accordance);

	/**
	 * @brief Destroys the object.
	 */
	~Compare();
};

