# pragma once
#include <algorithm>
#include <array>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
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
const double INFT = std::numeric_limits<double>::infinity();
const double PI = std::acos(-1);
static constexpr int KaseDefault = 100;
static constexpr double MinDeltaUV = 1e-6;
static constexpr double MinDist = 1e-4;
static constexpr double MinSquaredDist = 1e-8;
static constexpr double MinL1Dist = 1e-4;
static constexpr double PullVelocity = 1;
static constexpr double DeltaT = 1;

static constexpr double Epsilon = 1e-6;

static constexpr double MeantimeEpsilon = 1e-2;
static constexpr double SeparationDist = 1e-0;

std::normal_distribution<double> randNormal(0.0, 1.0); // 均值为0，标准差为1的正态分布
std::default_random_engine randGenerator(0);
std::uint64_t cnt;

bool SHOWANS = 1;
enum class BoundingBoxType { AABB, OBB, DOP14 };
enum class SolverType { TDIntv, BaseIntv, ManifoldBase, ManifoldTD }; //sampling, linearization
const BoundingBoxType BBDefault = BoundingBoxType::OBB;
const SolverType SolverDefault = SolverType::BaseIntv;
