#pragma once

#include <vector>
#include <array>

#include "../evo_core/types.hpp"

using Matrix = std::array<std::array<double, 4>, 4>;

Matrix create_matrix(double, double, double, double);
Matrix multiply_matrix(const Matrix&, const Matrix&);
Matrix get_forward_kinematics(const std::vector<double>&);

double get_articular(const std::vector<double>&, const std::vector<double>&);
std::vector<double> get_quaternion(const Matrix&);
double get_e_rot(const std::vector<double>&, const std::vector<double>&);

/*
* ==================================================
* Inverse Kinematics Fitness Evaluator (Functor)
* ==================================================
* This struct encapsulates the specific target state required to
* evaluate the inverse kinematics problem, while exposing an
* operator() that matches the generic 'Fitness' signature expected
* by the evolutionary engines.
*/
struct IKEvaluator
{
private:
    std::vector<double> target_xyz;
    std::vector<double> previous_posture;
    std::vector<double> target_quaternion;  // qw, qx, qy and qz
    double join_motion_penalty;
    double orientation_penalty;

public:
    explicit IKEvaluator(const std::vector<double>& target,
                         const std::vector<double>& prev_posture = {},
                         const std::vector<double>& target_q = {},
                         double jmp = 0.5,
                         double op = 10)
                        : target_xyz(target)
                        , previous_posture(prev_posture)
                        , target_quaternion(target_q)
                        , join_motion_penalty(jmp)
                        , orientation_penalty(op)
                    {}

    double operator()(const Genome& current_joints) const;
};
