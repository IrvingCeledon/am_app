#pragma once

#include <vector>

#include "../evo_core/types.hpp"

Matrix create_matrix(double, double, double, double);

Matrix multiply_matrix(const Matrix&, const Matrix&);

std::vector<double> get_xyz_position(const std::vector<double>&);

double calculate_ik(const std::vector<double>&, const std::vector<double>& target_xyz);
