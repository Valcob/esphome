#include "ntc.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ntc {

static const char *TAG = "ntc";

void NTC::setup() {
  this->sensor_->add_on_state_callback([this](float value) { this->process_(value); });
  if (this->sensor_->has_state())
    this->process_(this->sensor_->state);
}
void NTC::dump_config() { LOG_SENSOR("", "NTC Sensor", this) }
float NTC::get_setup_priority() const { return setup_priority::DATA; }
void NTC::process_(float value) {
  if (isnan(value)) {
    this->publish_state(NAN);
    return;
  }

  float lr = logf(value);
  float v = this->a_ + this->b_ * lr + this->c_ * lr * lr * lr;
  float temp = 1 / v - 273.15f;

  ESP_LOGD(TAG, "'%s' - Temperature: %.1f°C", this->name_.c_str(), temp);
  this->publish_state(temp);
}

}  // namespace ntc
}  // namespace esphome
