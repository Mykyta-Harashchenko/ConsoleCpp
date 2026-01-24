#include <iostream>
#include <unordered_map>
#include <functional>
#include <utility>

struct TemperatureChanged {
    double celsius;
};

class TemperatureSensor {
public:
    using Callback = std::function<void(const TemperatureChanged&)>;

    class Subscription {
    public:
        Subscription() = default;

        Subscription(TemperatureSensor* sensor, std::size_t id)
            : sensor_(sensor), id_(id) {
        }

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept { *this = std::move(other); }

        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                reset();
                sensor_ = other.sensor_;
                id_ = other.id_;
                other.sensor_ = nullptr;
                other.id_ = 0;
            }
            return *this;
        }

        ~Subscription() { reset(); }

        void reset() {
            if (sensor_) {
                sensor_->unsubscribe(id_); 
                sensor_ = nullptr;
                id_ = 0;
            }
        }

    private:
        TemperatureSensor* sensor_ = nullptr;
        std::size_t id_ = 0;
    };

    Subscription subscribe(Callback cb) {
        const std::size_t id = ++last_id_;
        observers_.emplace(id, std::move(cb));
        return Subscription(this, id);
    }

    void set_temperature(double celsius) {
        if (celsius_ == celsius) return;
        celsius_ = celsius;
        notify(TemperatureChanged{ celsius_ });
    }

private:
    friend class Subscription;

    void unsubscribe(std::size_t id) {
        observers_.erase(id);
    }

    void notify(const TemperatureChanged& ev) {
        auto snapshot = observers_;
        for (const auto& kv : snapshot) {
            const auto& cb = kv.second;
            if (cb) cb(ev);
        }
    }

    double celsius_ = 0.0;
    std::size_t last_id_ = 0;
    std::unordered_map<std::size_t, Callback> observers_;
};

struct ConsoleDisplay {
    void on_temperature(const TemperatureChanged& ev) {
        std::cout << "[UI] Temperature: " << ev.celsius << " C\n";
    }
};

struct OverheatAlarm {
    explicit OverheatAlarm(double threshold) : threshold_(threshold) {}

    void on_temperature(const TemperatureChanged& ev) {
        if (ev.celsius >= threshold_) {
            std::cout << "[ALARM] Overheat! (" << ev.celsius << " C)\n";
        }
    }

private:
    double threshold_;
};

int main() {
    TemperatureSensor sensor;

    ConsoleDisplay display;
    OverheatAlarm alarm{ 75.0 };

    auto sub_display = sensor.subscribe([&](const TemperatureChanged& ev) {
        display.on_temperature(ev);
        });

    auto sub_alarm = sensor.subscribe([&](const TemperatureChanged& ev) {
        alarm.on_temperature(ev);
        });

    sensor.set_temperature(20.0);
    sensor.set_temperature(80.0);

    sub_alarm.reset();

    sensor.set_temperature(90.0);

    return 0;
}