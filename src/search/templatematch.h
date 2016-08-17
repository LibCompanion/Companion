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
	 */
	TemplateMatch(string search_image_path, string compare_image_path, double accordance);

	/**
	 * @brief Destroys the object.
	 */
	~TemplateMatch();
};

