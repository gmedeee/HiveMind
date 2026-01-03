#pragma once

class Path {
    int *xs;
    int *ys;
    int len;
    int idx;

public:
    Path() : xs(0), ys(0), len(0), idx(0) {}

    void clear() {
        if (xs) 
        delete[] xs;
        if (ys) 
        delete[] ys;

        xs = 0; 
        ys = 0;
        len = 0; 
        idx = 0;
    }

    void set(int *px, int *py, int n) {
        clear();
        if (n <= 0) 
        return;

        xs = px;
        ys = py;
        len = n;
        idx = 0;
    }

    bool hasNext() const { 
        return idx < len; 
    }

    void peekNext(int &x, int &y) const {
        x = xs[idx];
        y = ys[idx];
    }

    void advance() { 
        if (idx < len) 
            idx++; 
    }

    ~Path() { 
        clear(); 
    }
};
