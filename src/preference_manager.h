#pragma once

#include <Preferences.h>
#include "config.h"

class PreferencesManager {
public:
    // Member variables for preferences, made public for easy access from other files
    int deadzone;
    float expoCurve;
    int maxOutput;
    int centerX;
    int centerY;

    // Constructor
    PreferencesManager();

    // Methods
    void begin();
    void saveAll();
    void loadAll();
private:
    Preferences prefs; // The actual Preferences object
};