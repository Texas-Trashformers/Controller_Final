// preference_manager.cpp
#include "preference_manager.h"
#include "config.h"

PreferencesManager::PreferencesManager() {}

void PreferencesManager::begin() {
  prefs.begin("controller", false);
  loadAll();
}

void PreferencesManager::saveAll() {
  prefs.putInt("deadzone", deadzone);
  prefs.putFloat("expo", expoCurve);
  prefs.putInt("maxout", maxOutput);
  prefs.putInt("centerX", centerX);
  prefs.putInt("centerY", centerY);
}

void PreferencesManager::loadAll() {
  deadzone = prefs.getInt("deadzone", DEFAULT_DEADZONE);
  expoCurve = prefs.getFloat("expo", DEFAULT_EXPO_CURVE);
  maxOutput = prefs.getInt("maxout", DEFAULT_MAX_OUTPUT);
  centerX = prefs.getInt("centerX", 2048);
  centerY = prefs.getInt("centerY", 2048);
}