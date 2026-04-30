#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <numbers>

#include "kinematic.hpp"

/*
 * ========================================
 * Homogeneous Transformation Matrix (DH)
 * ========================================
 *
 * General form:
 *
 * T_i =
 * [ cos(θ_i)  -sin(θ_i)*cos(α_i)   sin(θ_i)*sin(α_i)   b_i*cos(θ_i) ]
 * [ sin(θ_i)   cos(θ_i)*cos(α_i)  -cos(θ_i)*sin(α_i)   b_i*sin(θ_i) ]
 * [    0            sin(α_i)            cos(α_i)            d_i     ]
 * [    0               0                   0                 1      ]
 *
 * Interpretation:
 *
 * - First 3x3 block -> rotation matrix (orientation)
 * - Last column     -> position (translation)
 * - Last row        -> homogeneous coordinates
 *
 * Components:
 *
 *  cos(θ_i), sin(θ_i):
 *    Rotation around Z axis (joint rotation)
 *
 *  cos(α_i), sin(α_i):
 *    Rotation around X axis (link twist)
 *
 *  b_i*cos(θ_i), b_i*sin(θ_i):
 *    Projection of link length along X axis
 *
 *  d_i:
 *    Translation along Z axis
 *
 */
Matrix create_matrix(double teta, double alpha, double b, double d)
{
    double cos_teta = std::cos(teta);
    double sin_teta = std::sin(teta);

    double cos_alpha = std::cos(alpha);
    double sin_alpha = std::sin(alpha);

    Matrix M = {};

    M[0][0] = cos_teta;
    M[0][1] = -sin_teta * cos_alpha;
    M[0][2] = sin_teta * sin_alpha;
    M[0][3] = b * cos_teta;

    M[1][0] = sin_teta;
    M[1][1] = cos_teta * cos_alpha;
    M[1][2] = -cos_teta * sin_alpha;
    M[1][3] = b * sin_teta;

    M[2][0] = 0;
    M[2][1] = sin_alpha;
    M[2][2] = cos_alpha;
    M[2][3] = d;

    M[3][0] = 0;
    M[3][1] = 0;
    M[3][2] = 0;
    M[3][3] = 1;

    return M;
}

Matrix multiply_matrix(const Matrix& A, const Matrix& B)
{
    Matrix R = {};

    for (size_t i = 0; i < 4; ++i) {
        for (size_t k = 0; k < 4; ++k) {
            for (size_t j = 0; j < 4; ++j) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return R;
}

/*
 * =======================
 * Denavit-Hartenberg Table
 * =======================
 *
 * Each row represents the transformation between consecutive links (i-1 -> i)
 *
 * Columns:
 * θ_i (theta): joint angle (rotation around Z axis)
 * α_i (alpha): twist angle (rotation around X axis)
 * b_i: link length (translation along X axis)
 * d_i: link offset (translation along Z axis)
 *
 * Table used:
 *
 * Link 1:
 * θ_1 = q1          -> rotational joint (variable)
 * α_1 = -π/2        -> fixed twist
 * b_1 = 0.5         -> constant link length
 * d_1 = 0           -> no offset
 *
 * Link 2:
 * θ_2 = q2          -> rotational joint (variable)
 * α_2 = π/2         -> fixed twist
 * b_2 = 0.4         -> constant link length
 * d_2 = 0
 *
 * Link 3:
 * θ_3 = 0           -> no rotation
 * α_3 = 0           -> no twist
 * b_3 = 0           -> no link length
 * d_3 = q3          -> prismatic joint (variable translation)
 *
 * Link 4:
 * θ_4 = q4          -> rotational joint (variable)
 * α_4 = -π/2        -> fixed twist
 * b_4 = 0
 * d_4 = 0
 *
 * Link 5:
 * θ_5 = q5          -> rotational joint (variable)
 * α_5 = π/2         -> fixed twist
 * b_5 = 0.3         -> constant link length
 * d_5 = 0
 *
 *  ========================================
 *  Final Transformation
 *  ========================================
 *
 *  The end-effector pose is obtained by multiplying all matrices:
 *
 *  T = T1 * T2 * T3 * T4 * T5
 *
 *  The final position (x, y, z) is extracted from:
 *
 *  x = T[0][3]
 *  y = T[1][3]
 *  z = T[2][3]
 *
 *  ========================================
 *  Important Notes
 *  ========================================
 *  - q1, q2, q4, q5 are revolute joints (angles in radians)
 *  - q3 is a prismatic joint (linear displacement in meters)
 *  - Joint limits:
 *     revolute: [-π, π]
 *     prismatic: [0.1, 1.0]
 *  - Matrix multiplication is NOT commutative:
 *    T1 * T2 != T2 * T1
 *  - The order strictly follows the kinematic chain
 *  - Each matrix encodes both rotation and translation
 *  - The result is always a 4x4 matrix, NOT a scalar
 */
Matrix get_forward_kinematics(const std::vector<double>& q_values)
{
    double q1 = q_values.at(0);
    double q2 = q_values.at(1);
    double q3 = q_values.at(2);
    double q4 = q_values.at(3);
    double q5 = q_values.at(4);

    const double half_pi = std::numbers::pi/2;

    Matrix T1 = create_matrix(q1, -half_pi, 0.5, 0.0);
    Matrix T2 = create_matrix(q2, half_pi, 0.4, 0.0);
    Matrix T3 = create_matrix(0.0, 0.0, 0.0, q3);
    Matrix T4 = create_matrix(q4, -half_pi, 0.0, 0.0);
    Matrix T5 = create_matrix(q5, half_pi, 0.3, 0.0);

    Matrix T = multiply_matrix(T1, T2);
    T = multiply_matrix(T, T3);
    T = multiply_matrix(T, T4);
    T = multiply_matrix(T, T5);

    return T;
}

double get_articular(const std::vector<double>& q_olds, const std::vector<double>& q_news)
{
    // qn_difference
    double q1_d = q_news[0] - q_olds[0];
    double q2_d = q_news[1] - q_olds[1];
    double q3_d = q_news[2] - q_olds[2];
    double q4_d = q_news[3] - q_olds[3];
    double q5_d = q_news[4] - q_olds[4];

    return std::sqrt( q1_d*q1_d
                    + q2_d*q2_d
                    + q3_d*q3_d
                    + q4_d*q4_d
                    + q5_d*q5_d
                );
}

std::vector<double> get_quaternion(const Matrix& K)
{
    // Rotation Matrix Trace
    std::vector<double> actual_quaternion(4);
    double tr = K[0][0] + K[1][1] + K[2][2];
    double max_val = std::max({K[0][0], K[1][1], K[2][2]});
    double s(0.0);

    if (tr > 0) {
        s = 2.0 * std::sqrt(tr + 1.0);
        actual_quaternion[0] = 0.25 * s;
        actual_quaternion[1] = (K[2][1] - K[1][2]) / s;
        actual_quaternion[2] = (K[0][2] - K[2][0]) / s;
        actual_quaternion[3] = (K[1][0] - K[0][1]) / s;
    }
    else if (tr <= 0 && K[0][0] == max_val) {
        s = 2.0 * std::sqrt(1 + K[0][0] - K[1][1] - K[2][2]);
        actual_quaternion[0] = (K[2][1] - K[1][2]) / s;
        actual_quaternion[1] = 0.25 * s;
        actual_quaternion[2] = (K[0][1] + K[1][0]) / s;
        actual_quaternion[3] = (K[0][2] + K[2][0]) / s;
    }
    else if (tr <= 0 && K[1][1] == max_val) {
        s = 2.0 * std::sqrt(1 + K[1][1] - K[0][0] - K[2][2]);
        actual_quaternion[0] = (K[0][2] - K[2][0]) / s;
        actual_quaternion[1] = (K[0][1] + K[1][0]) / s;
        actual_quaternion[2] = 0.25 * s;
        actual_quaternion[3] = (K[1][2] + K[2][1]) / s;
    }
    else if (tr <= 0 && K[2][2] == max_val) {
        s = 2.0 * std::sqrt(1 + K[2][2] - K[0][0] - K[1][1]);
        actual_quaternion[0] = (K[1][0] - K[0][1]) / s;
        actual_quaternion[1] = (K[0][2] + K[2][0]) / s;
        actual_quaternion[2] = (K[1][2] + K[2][1]) / s;
        actual_quaternion[3] = 0.25 * s;
    }

    return actual_quaternion;
}

double get_e_rot(const std::vector<double>& actual_quaternion, const std::vector<double>& target_quaternion)
{
    double qw_dot = actual_quaternion[0] * target_quaternion[0];
    double qx_dot = actual_quaternion[1] * target_quaternion[1];
    double qy_dot = actual_quaternion[2] * target_quaternion[2];
    double qz_dot = actual_quaternion[3] * target_quaternion[3];

    return 1.0 - std::abs(qw_dot + qx_dot + qy_dot + qz_dot);
}

// Inverse Kinematic
// fitness:  f(q) = 1 / 1 + e
// Where e is the euclidean distance between actual position and final position
double IKEvaluator::operator()(const Genome& current_joints) const
{
    Matrix K = get_forward_kinematics(current_joints);

    double dx = K[0][3] - target_xyz[0];
    double dy = K[1][3] - target_xyz[1];
    double dz = K[2][3] - target_xyz[2];

    double e_espacial = std::sqrt(dx*dx + dy*dy + dz*dz);

    double jmp(0.0);
    if (!previous_posture.empty()) {
        jmp = join_motion_penalty * get_articular(previous_posture, current_joints);
    }

    double op(0.0);
    if (!target_quaternion.empty()) {
        op = get_e_rot(get_quaternion(K), target_quaternion) * orientation_penalty;
    }

    return 1.0 / (1.0 + e_espacial + jmp + op);
}
