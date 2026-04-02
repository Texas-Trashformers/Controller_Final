// preference_manager.cpp
#include "preference_manager.h"
#include "config.h"

// Global preferences instance
PreferencesManager prefs;

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
  prefs.putInt("minRX", minRX);
  prefs.putInt("maxRX", maxRX);
  prefs.putInt("minRY", minRY);
  prefs.putInt("maxRY", maxRY);
}

void PreferencesManager::loadAll() {
  deadzone = prefs.getInt("deadzone", DEFAULT_DEADZONE);
  expoCurve = prefs.getFloat("expo", DEFAULT_EXPO_CURVE);
  maxOutput = DEFAULT_MAX_OUTPUT;  // Always use the compile-time default (ignores stale NVS)
  centerLX = prefs.getInt("centerLX", 2048);
  centerLY = prefs.getInt("centerLY", 2048);
  centerRX = prefs.getInt("centerRX", 2048);
  centerRY = prefs.getInt("centerRY", 2048);
  minRX    = prefs.getInt("minRX",    0);
  maxRX    = prefs.getInt("maxRX",    4095);
  minRY    = prefs.getInt("minRY",    0);
  maxRY    = prefs.getInt("maxRY",    4095);
}