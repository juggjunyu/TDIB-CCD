#include <random>

static constexpr double MinDeltaUV = 1e-6;
static constexpr double MinSquaredDist = 1e-12;
static constexpr double Epsilon = 1e-6;
static constexpr double DeltaT = 1;
bool DEBUG = 0;
static constexpr bool SHOWANS = 0;
enum class BB { AABB, OBB };

std::normal_distribution<double> randNormal(0.0, 1.0); // 均值为0，标准差为1的正态分布

std::uint64_t cnt;
