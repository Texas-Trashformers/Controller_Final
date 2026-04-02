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
  prefs.putInt("centerLX", centerLX);
  prefs.putInt("centerLY", centerLY);
  prefs.putInt("centerRX", centerRX);
  prefs.putInt("centerRY", centerRY);
}

void PreferencesManager::loadAll() {
  deadzone = prefs.getInt("deadzone", DEFAULT_DEADZONE);
  expoCurve = prefs.getFloat("expo", DEFAULT_EXPO_CURVE);
  maxOutput = prefs.getInt("maxout", DEFAULT_MAX_OUTPUT);
  centerLX = prefs.getInt("centerLX", 2048);
  centerLY = prefs.getInt("centerLY", 2048);
  centerRX = prefs.getInt("centerRX", 2048);
  centerRY = prefs.getInt("centerRY", 2048);
}