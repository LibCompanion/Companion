//
// Created by andre on 27.11.2016.
//

#include "Comparison.h"


Comparison::Comparison(double accordance) {
    this->accordance = accordance;
}

Comparison::~Comparison() {}

double Comparison::getAccordance() {
    return accordance;
}

void Comparison::setAccordance(double accordance) {
    Comparison::accordance = accordance;
}
