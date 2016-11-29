//
// Created by andre on 27.11.2016.
//

#ifndef COMPANION_COMPARISON_H
#define COMPANION_COMPARISON_H


class Comparison {

    public:
        Comparison(double accordance);

        ~Comparison();

        double getAccordance();

        void setAccordance(double accordance);

private:

        double accordance;

};


#endif //COMPANION_COMPARISON_H
