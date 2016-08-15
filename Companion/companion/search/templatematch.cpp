#include "templatematch.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TemplateMatch::TemplateMatch()
{
	this->search_image_path = "";
	this->compare_image_path = "";
	this->accordance = 0;
}

TemplateMatch::TemplateMatch(string search_image_path, string compare_image_path, double accordance)
{
	this->search_image_path = search_image_path;
	this->compare_image_path = compare_image_path;
	this->accordance = accordance;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TemplateMatch::~TemplateMatch()
{
}
