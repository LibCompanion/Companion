#include "search.h"

void Search::set_search_image_path(string search_image_path)
{
	this->search_image_path = search_image_path;
}

string Search::get_search_image_path()
{
	return this->search_image_path;
}

void Search::set_compare_image_path(string compare_image_path)
{
	this->compare_image_path = compare_image_path;
}

string Search::get_compare_image_path()
{
	return this->compare_image_path;
}

void Search::set_accordance(double accordance)
{
	this->accordance = accordance;
}

double Search::get_accordance()
{
	return this->accordance;
}