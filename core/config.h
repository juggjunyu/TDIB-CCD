# pragma once
#include <algorithm>
#include <array>
#include <string>
#include <cstdint>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <queue>
#include <set>
#include <span>
#include <chrono>
#include <cmath>
#include <random>

#include <Eigen/Dense>

using Eigen::Array2d;
using Eigen::Vector3d;
using Eigen::Vector4d;
using Eigen::MatrixXd;
using Array2dError = std::pair<Array2d, Array2d>;
const double INFT = std::numeric_limits<double>::infinity();
const double PI = std::acos(-1);
const double MachineEps = DBL_EPSILON;//std::DBL_EPSILON;
const double SqrtMachineEps = std::sqrt(MachineEps); //a hyperparam, not for error calculation, so no need to be so precise

static constexpr int KaseDefault = 100;
static constexpr double MinDeltaUV = 1e-6;
static constexpr double MinDist = 1e-4;
static constexpr double MinSquaredDist = 1e-8;
static constexpr double MinL1Dist = 1e-3;
static constexpr double PullVelocity = 1;
static constexpr double DeltaT = 1;

static constexpr double Epsilon = 1e-6;

static constexpr double MeantimeEpsilon = 1e-3;
static constexpr double SeparationEucDist = 1e-0;
// static constexpr double SeparationUVDist = 1e-0;

std::normal_distribution<double> randNormal(0.0, 1.0); // 均值为0，标准差为1的正态分布
std::default_random_engine randGenerator(0);
std::uint64_t cnt;

bool SHOWANS = 0;
enum class BoundingBoxType { AABB, OBB, DOP14 };
enum class SolverType { TDIntv, BaseIntv, ManifoldBase, ManifoldTD }; //sampling, linearization
BoundingBoxType BBDefault = BoundingBoxType::OBB;
SolverType SolverDefault = SolverType::BaseIntv;
