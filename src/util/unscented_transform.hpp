#pragma once

// NOT DONE

#include "gaussian.hpp"

template <typename T = double>
class UnscentedTransform : public Gaussian<T>
{
private:
    /* data */
public:
    UnscentedTransform(Gaussian & p)
    {
        mu = p.mean();
        S = p.sqrt_cov();
    }

    ~UnscentedTransform(); 

    template <typename Func>
    Gaussian<T> transform(Func h)
    {
        const size_t & n = size();

        Eigen::MatrixXd p(n,n);
        p = std::sqrt(n)*S;
        Eigen::MatrixXd s(n,n);
        s = mu.replicate(1, n) + p;
        Eigen::MatrixXd s_s(n, 2*n);
        s_s << s, mu.replicate(1, n) - p;

        std::cout << "s_s: " << s_s << std::endl;
    }
};


