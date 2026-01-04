#pragma once

template <typename T>
T cellDiv(T a, T b) {
    return (a + b - 1) / b; // b > 0
}

template <typename T>
T absT(T x) {
    if (x < 0) return -x;
    return x;
}
