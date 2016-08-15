#pragma once

#include "search.h"
#include <iostream>

using namespace std;

class Flann : public Search
{
public:
	Flann();

	Flann(string search_image_path, string compare_image_path, double accordance, int matches_size);

	~Flann();

	void set_matches_size(int matches_size);

	int get_matches_size();

private:

	int matches_size;
};

