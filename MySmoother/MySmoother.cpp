//
//  MySmoother.cpp
//  MySmoother
//

#include "MySmoother.pch"
//#include <string>
#define INIT_VALUE -273.15  // tekitou
#define ALPHA_DEFAULT 0.4
#define GAMMA_DEFAULT 0.5

// Double Exponential Smoothing
// http://nbviewer.jupyter.org/github/wingcloud/notebooks/blob/master/SmoothingFilter1.ipynb
class Smoother {
public:
    Smoother(float alpha, float gamma) {
        alpha_ = alpha;
        gamma_ = gamma;
    }
    
protected:
    float alpha_;
    float gamma_;
};

class Smoother1D : public Smoother {
public:
    Smoother1D(float alpha = ALPHA_DEFAULT, float gamma = GAMMA_DEFAULT) : Smoother(alpha, gamma) {
        last_smoothed_ = last_b_ = INIT_VALUE;
    }
    
    float smooth(float input) {
        if (last_smoothed_ == INIT_VALUE) {
            last_smoothed_ = last_b_ = input;
            return input;
        }
        
        float smoothed = alpha_*input + (1-alpha_)*(last_smoothed_+last_b_);
        float b = gamma_*(smoothed-last_smoothed_) + (1-gamma_)*last_b_;
        last_smoothed_ = smoothed;
        last_b_ = b;
        
        return smoothed;
    }
    
private:
    float last_smoothed_;
    float last_b_;
};

class Smoother2D : public Smoother {
public:
    Smoother2D(float alpha = ALPHA_DEFAULT, float gamma = GAMMA_DEFAULT) : Smoother(alpha, gamma) {
        x_smoother_ = new Smoother1D(alpha, gamma);
        y_smoother_ = new Smoother1D(alpha, gamma);
    }
    
    ~Smoother2D() {
        if (!x_smoother_) {
            delete x_smoother_;
        }
        if (!y_smoother_) {
            delete y_smoother_;
        }
    }
    
    Vector2 smooth(Vector2 input) {
        Vector2 result;
        result.x = x_smoother_->smooth(input.x);
        result.y = y_smoother_->smooth(input.y);
        
        return result;
    }
    
private:
    Smoother1D* x_smoother_;
    Smoother1D* y_smoother_;
};

class Smoother3D : public Smoother {
public:
    Smoother3D(float alpha = ALPHA_DEFAULT, float gamma = GAMMA_DEFAULT) : Smoother(alpha, gamma) {
        x_smoother_ = new Smoother1D(alpha, gamma);
        y_smoother_ = new Smoother1D(alpha, gamma);
        z_smoother_ = new Smoother1D(alpha, gamma);
    }
    
    ~Smoother3D() {
        if (!x_smoother_) {
            delete x_smoother_;
        }
        if (!y_smoother_) {
            delete y_smoother_;
        }
        if (!z_smoother_) {
            delete z_smoother_;
        }
    }
    
    Vector3 smooth(Vector3 input) {
        Vector3 result;
        result.x = x_smoother_->smooth(input.x);
        result.y = y_smoother_->smooth(input.y);
        result.z = z_smoother_->smooth(input.z);
        
        return result;
    }
    
private:
    Smoother1D* x_smoother_;
    Smoother1D* y_smoother_;
    Smoother1D* z_smoother_;
};

void* getSmoother1D() {
    auto smoother1d = new Smoother1D();
    return static_cast<void*>(smoother1d);
}
void* getSmoother2D() {
    auto smoother2d = new Smoother2D();
    return static_cast<void*>(smoother2d);
}
void* getSmoother3D() {
    auto smoother3d = new Smoother3D();
    return static_cast<void*>(smoother3d);
}
float smooth1D(void* smoother, float f) {
    auto smoother1d = static_cast<Smoother1D*>(smoother);
    return smoother1d->smooth(f);
}
Vector2 smooth2D(void* smoother, Vector2 p) {
    auto smoother2d = static_cast<Smoother2D*>(smoother);
    return smoother2d->smooth(p);
}
Vector3 smooth3D(void* smoother, Vector3 p) {
    auto smoother3d = static_cast<Smoother3D*>(smoother);
    return smoother3d->smooth(p);
}

//void toString(Vector2 p, char* dest) {
//    std::string src = "(x, y) = (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
//    strcpy(dest, src.c_str());
//}

