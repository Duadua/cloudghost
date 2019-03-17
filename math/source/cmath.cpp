#include "cmath.h"
#include <cmath>

const float CMath::pi = static_cast<float>(acos(-1));
const float CMath::eps = static_cast<float>(1e-6);

float CMath::rad_to_deg(float rad) { return (180.0f / pi)*rad; }
float CMath::deg_to_rad(float deg) { return (pi / 180.0f) * deg; }

float CMath::interp_linear(float v, float min, float max) { return min + v * (max - min); }

int CMath::fcmp(float a, float b) {
    if(a-b < -eps) return -1;
    else if(a-b > eps) return 1;
    else return 0;
}
