#include "flann.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Flann::Flann()
{
	this->search_image_path = "";
	this->compare_image_path = "";
	this->accordance = 1;
	this->matches_size = 0;
}

Flann::Flann(string search_image_path, string compare_image_path, double accordance, int matches_size)
{
	this->search_image_path = search_image_path;
	this->compare_image_path = compare_image_path;
	this->accordance = accordance;
	this->matches_size = matches_size;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Flann::~Flann()
{
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public Methods                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void Flann::set_matches_size(int matches_size)
{
	this->matches_size = matches_size;
}

int Flann::get_matches_size()
{
	return this->matches_size;
}
