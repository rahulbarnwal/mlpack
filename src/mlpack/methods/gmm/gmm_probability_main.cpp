/**
 * @file gmm_probability_main.cpp
 * @author Ryan Curtin
 *
 * Given a GMM, calculate the probability of points coming from it.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/cli.hpp>
#include <mlpack/core/util/mlpack_main.hpp>
#include "gmm.hpp"

using namespace std;
using namespace mlpack;
using namespace mlpack::gmm;
using namespace mlpack::util;

PROGRAM_INFO("GMM Probability Calculator",
    "This program calculates the probability that given points came from a "
    "given GMM (that is, P(X | gmm)).  The GMM is specified with the " +
    PRINT_PARAM_STRING("input_model") + " parameter, and the points are "
    "specified with the " + PRINT_PARAM_STRING("input") + " parameter.  The "
    "output probabilities may be saved via the " +
    PRINT_PARAM_STRING("output") + " output parameter."
    "\n\n"
    "So, for example, to calculate the probabilities of each point in " +
    PRINT_DATASET("points") + " coming from the pre-trained GMM " +
    PRINT_MODEL("gmm") + ", while storing those probabilities in " +
    PRINT_DATASET("probs") + ", the following command could be used:"
    "\n\n" +
    PRINT_CALL("gmm_probability", "input_model", "gmm", "input", "points",
        "output", "probs"));

PARAM_MODEL_IN_REQ(GMM, "input_model", "Input GMM to use as model.", "m");
PARAM_MATRIX_IN_REQ("input", "Input matrix to calculate probabilities of.",
    "i");

PARAM_MATRIX_OUT("output", "Matrix to store calculated probabilities in.", "o");

void mlpackMain()
{
  RequireAtLeastOnePassed({ "output" }, false, "no results will be saved");

  // Get the GMM and the points.
  GMM gmm = std::move(CLI::GetParam<GMM>("input_model"));

  arma::mat dataset = std::move(CLI::GetParam<arma::mat>("input"));

  // Now calculate the probabilities.
  arma::rowvec probabilities(dataset.n_cols);
  for (size_t i = 0; i < dataset.n_cols; ++i)
    probabilities[i] = gmm.Probability(dataset.unsafe_col(i));

  // And save the result.
  if (CLI::HasParam("output"))
    CLI::GetParam<arma::mat>("output") = std::move(probabilities);
}
