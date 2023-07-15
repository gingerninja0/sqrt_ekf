#pragma once

#include <Eigen/Dense>


template <typename T = double>
class Gaussian
{
private:
    Eigen::VectorX<T> mu;
    Eigen::MatrixX<T> S;
public:
    Gaussian(/* args */);
    ~Gaussian();
};


#include "gaussian.hpp"
