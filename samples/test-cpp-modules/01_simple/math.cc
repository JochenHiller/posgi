export module math;

import <vector>;
import <numeric>;

export int add(int fir, int sec) {
  return fir + sec;
}

export int getProduct(const std::vector<int>& vec) {
  return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}
