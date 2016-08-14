#pragma once

#include "search.h"
#include <iostream>

class Compare : public Search
{
public:
	Compare();

	Compare(string search_image_path, string compare_image_path, double accordance);

	~Compare();
};

