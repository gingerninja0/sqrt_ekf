
#include <iostream>
#include <vector>

#include "../util/gaussian.hpp"

int main()
{
    std::cout << "Gaussian" << std::endl;

    Eigen::MatrixXd S(3,3);
    Eigen::VectorXd mu(3);

    mu << 0,
          1,
          2;

    S << 1, 2, 3,
         0, 5, 6,
         0, 0, 7;

    Gaussian<> test(mu,S);

    std::cout << test.mean() << std::endl;
    std::cout << test.sqrt_cov() << std::endl;
    std::cout << std::endl;

    // Marginal

    Gaussian<> test_marginal;
    Eigen::VectorXd idx(2);
    idx << 0, 2;
    test_marginal = test.marginal(idx);

    std::cout << test_marginal.mean() << std::endl;
    std::cout << test_marginal.sqrt_cov() << std::endl;
    std::cout << std::endl;

    return 0;
}