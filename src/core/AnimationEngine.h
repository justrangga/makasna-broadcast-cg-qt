#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <cmath>

class AnimationEngine : public QObject {
    Q_OBJECT
public:
    explicit AnimationEngine(QObject *parent = nullptr);

    enum class EasingType {
        Linear,
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseOutBack,
        EaseOutElastic,
        EaseOutBounce
    };
    Q_ENUM(EasingType)

    Q_INVOKABLE static double evaluateEasing(const QString &easingName, double t);
    Q_INVOKABLE static double interpolate(double a, double b, double t, const QString &easingName = "linear");
};
