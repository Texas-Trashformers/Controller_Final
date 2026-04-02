#pragma once

#include <Preferences.h>
#include "config.h"

class PreferencesManager {
public:
    // Member variables for preferences, made public for easy access from other files
    int deadzone;
    float expoCurve;
    int maxOutput;
    int centerLX;
    int centerLY;
    int centerRX;
    int centerRY;
    int minRX;
    int maxRX;
    int minRY;
    int maxRY;

    // Constructor
    PreferencesManager();

    // Methods
    void begin();
    void saveAll();
    void loadAll();
private:
    Preferences prefs; // The actual Preferences object
};

// Global preferences instance
extern PreferencesManager prefs;