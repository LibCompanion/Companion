#pragma once

#include "search.h"
#include <iostream>

using namespace std;

class TemplateMatch : public Search
{
public:
	TemplateMatch();

	TemplateMatch(string search_image_path, string compare_image_path, double accordance);

	~TemplateMatch();
};

