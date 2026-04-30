#include <cmath>
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
std::vector<double> get_xyz_position(const std::vector<double>& q_values)
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

    std::vector<double> pos(3);
    pos[0] = T[0][3];
    pos[1] = T[1][3];
    pos[2] = T[2][3];

    return pos;
}

// Inverse Kinematic
// fitness:  f(q) = 1 / 1 + e
// Where e is the euclidian distance between actual position and final position
double calculate_ik(const std::vector<double>& position, const std::vector<double>& target_xys)
{
    double p_x = position.at(0);
    double p_y = position.at(1);
    double p_z = position.at(2);

    double t_x = target_xys.at(0);
    double t_y = target_xys.at(1);
    double t_z = target_xys.at(2);

    double term_1 = (p_x - t_x) * (p_x - t_x);
    double term_2 = (p_y - t_y) * (p_y - t_y);
    double term_3 = (p_z - t_z) * (p_z - t_z);

    double e = std::sqrt(term_1 + term_2 + term_3);

    return 1 / (1 + e);
}
