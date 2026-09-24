#include "AnimationEngine.h"
#include <QtMath>
#include <algorithm>

AnimationEngine::AnimationEngine(QObject *parent) : QObject(parent) {}

double AnimationEngine::evaluateEasing(const QString &easingName, double t) {
    t = std::clamp(t, 0.0, 1.0);

    if (easingName == "easeInQuad") {
        return t * t;
    } else if (easingName == "easeOutQuad") {
        return t * (2.0 - t);
    } else if (easingName == "easeInOutQuad") {
        return t < 0.5 ? 2.0 * t * t : -1.0 + (4.0 - 2.0 * t) * t;
    } else if (easingName == "easeInCubic") {
        return t * t * t;
    } else if (easingName == "easeOutCubic") {
        double t1 = t - 1.0;
        return t1 * t1 * t1 + 1.0;
    } else if (easingName == "easeInOutCubic") {
        return t < 0.5 ? 4.0 * t * t * t : (t - 1.0) * (2.0 * t - 2.0) * (2.0 * t - 2.0) + 1.0;
    } else if (easingName == "easeOutBack") {
        double c1 = 1.70158;
        double c3 = c1 + 1.0;
        return 1.0 + c3 * std::pow(t - 1.0, 3) + c1 * std::pow(t - 1.0, 2);
    } else if (easingName == "easeOutElastic") {
        if (t == 0.0) return 0.0;
        if (t == 1.0) return 1.0;
        double c4 = (2.0 * M_PI) / 3.0;
        return std::pow(2.0, -10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
    } else if (easingName == "easeOutBounce") {
        double n1 = 7.5625;
        double d1 = 2.75;
        if (t < 1.0 / d1) {
            return n1 * t * t;
        } else if (t < 2.0 / d1) {
            double t1 = t - 1.5 / d1;
            return n1 * t1 * t1 + 0.75;
        } else if (t < 2.5 / d1) {
            double t1 = t - 2.25 / d1;
            return n1 * t1 * t1 + 0.9375;
        } else {
            double t1 = t - 2.625 / d1;
            return n1 * t1 * t1 + 0.984375;
        }
    }
    return t; // linear fallback
}

double AnimationEngine::interpolate(double a, double b, double t, const QString &easingName) {
    double progress = evaluateEasing(easingName, t);
    return a + (b - a) * progress;
}
