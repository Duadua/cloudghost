#include "cmath.h"
#include <cmath>

const float CMath::pi = acos(-1);
const float CMath::eps = 1e-6;

float CMath::rad_to_deg(float rad) { return (180.0f / pi)*rad; }
float CMath::deg_to_rad(float deg) { return (pi / 180.0f) * deg; }

float CMath::interp_linear(float v, float min, float max) { return min + v * (max - min); }
