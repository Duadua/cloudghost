#pragma once

#include <memory>
#include <chrono>

using uint = unsigned int;
using ulong = unsigned long;
using uchar = unsigned char;
using ll = long long;

using UPTR_char = std::unique_ptr<char>;
using SPTR_char = std::shared_ptr<char>;
using WPTR_char = std::weak_ptr<char>;
using UPTR_uchar = std::unique_ptr<uchar>;
using SPTR_uchar = std::shared_ptr<uchar>;
using WPTR_uchar = std::weak_ptr<uchar>;

using UPTR_float = std::unique_ptr<float>;
using SPTR_float = std::shared_ptr<float>;
using WPTR_float = std::weak_ptr<float>;

using hour_type = std::chrono::duration<ll, std::ratio<60 * 60, 1>>;
using minute_type = std::chrono::duration<ll, std::ratio<60, 1>>;
using second_type = std::chrono::duration<ll, std::ratio<1, 1>>;
using msecnd_type = std::chrono::duration<ll, std::ratio<1, 1000>>;

using system_time_point = std::chrono::system_clock::time_point;
using highrs_time_point = std::chrono::high_resolution_clock::time_point;