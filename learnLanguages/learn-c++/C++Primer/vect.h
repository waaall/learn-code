#ifndef VECTOR_H //这个名称无所谓
#define VECTOR_H 

#include <iostream>

namespace VECTOR
{
    class VECTOR
    {
    private:
        double x;
        double y;
        double mag;
        double ang;
        Mode mode;

        //private methods for setting values
        void set_mag();
        void set_ang();
        void set_x();
        void set_y();
    public:
        VECTOR();
        VECTOR(double n1, double n2, Mode form = RECT);
        ~VECTOR();

        double xval() const {return x;}
        double yval() const {return y;}
        double magval() const {return mag;}
        double angval() const {return ang;}
        void polar_mode();
        void rect_mode();

        //oprator overloading
        Vector operator+(const Vector & b) const;
        Vector operator-(const Vector & b) const;        
        Vector operator-() const;
        Vector operator*(double n) const;
        //friends
        friend Vector oprator*(double n, const Vector & a);
        friend std::ostream & operator<<(std::ostream & os, const Vector & v);        

    };
}

#endif