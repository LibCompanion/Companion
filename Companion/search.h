#pragma once

#include <iostream>

using namespace std;

class Search
{
public:

	void set_search_image_path(string search_image_path);

	string get_search_image_path();

	void set_compare_image_path(string compare_image_path);

	string get_compare_image_path();

	void set_accordance(double accordance);

	double get_accordance();

protected:

	string search_image_path;

	string compare_image_path;

	double accordance;
};

