#pragma once

#include <cstddef>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/QR>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

template <typename T = double>
class Gaussian
{
private:

    Eigen::VectorX<T> mu;
    Eigen::MatrixX<T> S;

public:

    // Constructors and Destructor

    Gaussian()
    {

    }

    Gaussian(std::size_t & n)
        : mu(n), S(n,n)
    {

    }

    template <typename Y>
    Gaussian(const Eigen::VectorX<Y> & mu, const Eigen::MatrixX<Y> & S)
        : mu(mu.template cast<T>()), S(S.template cast<T>())
    {
        assert(S.rows() == S.cols());
        assert(mu.rows() == S.rows());
    }

    template <typename Y> friend class Gaussian;

    template <typename Y>
    Gaussian(const Gaussian<Y> & p)
        : mu(p.mu.template cast<T>()), S(p.S.template cast<T>())
    {
        assert(S.rows() == S.cols());
        assert(mu.rows() == S.rows());
    }

    template <typename Y>
    Gaussian<Y> cast() const
    {
        return Gaussian<Y>(*this);
    }

    ~Gaussian()
    {

    }

    // Setters

    void set_mu(Eigen::VectorX<T> & mu_)
    {
        mu = mu_;
    }

    void set_cov(Eigen::MatrixX<T> & S_)
    {
        S = S_;
    }

    // Query

    std::size_t size(void) const
    {
        return mu.size();
    }

    Eigen::VectorX<T> & mean(void) 
    {
        return mu;
    }

    const Eigen::VectorX<T> & mean(void) const
    {
        return mu;
    }

    Eigen::MatrixX<T> & sqrt_cov(void) 
    {
        return S;
    }

    const Eigen::MatrixX<T> & sqrt_cov(void) const
    {
        return S;
    }

    Eigen::MatrixX<T> cov(void) const
    {
        return S.transpose() * S;
    }

    // Marginal and Conditional

    template <typename Idx>
    Gaussian marginal(const Idx & idx) const
    {
        Gaussian out;
        out.mu = mu(idx);
        
        // Extract all rows of idxth cols of S
        Eigen::MatrixX<T> A = S(Eigen::all,idx);

        // For S to be upper triangular by performing qr decomposition
        Eigen::HouseholderQR<Eigen::MatrixX<T>> qr(A);

        A = qr.matrixQR().template triangularView<Eigen::Upper>(); 
        // S is now upper triangular as required
        out.S = A.topRows(A.cols());
        return out;
    }

    // Given joint density p(x), return conditional density p(x(idxA) | x(idxB) = xB)
    template <typename IdxA, typename IdxB>
    Gaussian conditional(const IdxA & idxA, const IdxB & idxB, const Eigen::VectorX<T> & xB) const
    {
        Gaussian out;
        Eigen::MatrixX<T> SB, SA, R;

        SA = S(Eigen::all, idxA);
        SB = S(Eigen::all, idxB);

        Eigen::MatrixX<T> S(SA.rows(),SA.cols()+SB.cols());

        S << SB, SA;

        // For S to be upper triangular by performing qr decomposition 
        Eigen::HouseholderQR<Eigen::MatrixX<T>> qr(S);

        R = qr.matrixQR().template triangularView<Eigen::Upper>(); 
    
        out.mu = mu(idxA) +
            R.topRightCorner(idxB.size(),idxA.size()).transpose()*
            R.topLeftCorner(idxB.size(),idxB.size()).eval().template triangularView<Eigen::Upper>().transpose().solve(xB - mu(idxB));
        out.S = R.bottomRightCorner(idxA.size(),idxA.size());
    
        return out;
    }

    // Plot 2D ellipse

    Eigen::Matrix<T,2,Eigen::Dynamic> confidenceEllipse(int nSigma = 3, int nSamples = 100) const
    {
        assert(nSigma == 3); // Other values not currently implemented
        const std::size_t & n = size();
        assert(n == 2);

        T X22 = 11.829158081900795;
        
        T r = std::sqrt(X22);

        Eigen::Matrix<T,2,Eigen::Dynamic> X;
        X.resize(2,nSamples);

        Eigen::MatrixX<T> w(2,nSamples);
        T t;

        for (int i = 0; i < nSamples; i++)
        {
            t = 2*M_PI*(T(i)/T(nSamples-1));
            
            w(0,i) = r*cos(t);
            w(1,i) = r*sin(t);
        }

        X = S.transpose()*w + mu.replicate(1,nSamples);  

        //std::cout << "S =\n" << S << "\n" << std::endl;

        assert(X.cols() == nSamples);
        assert(X.rows() == 2);

        return X;
    }

};


#include "gaussian.hpp"
