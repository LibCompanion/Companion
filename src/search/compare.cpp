#include "compare.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Compare::Compare()
{
	this->search_image_path = "";
	this->compare_image_path = "";
	this->accordance = 0;
}

Compare::Compare(string search_image_path, string compare_image_path, double accordance)
{
	this->search_image_path = search_image_path;
	this->compare_image_path = compare_image_path;
	this->accordance = accordance;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Compare::~Compare()
{
}