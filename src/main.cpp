#include "main.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// Increase haptic intensity by 1.5x (50% stronger rumble)
float hapticForceMultiplier = 1.5f;

// Hook the core HapticPresetSO used by modern Quest patches
MAKE_HOOK_MATCH(HapticPresetSO_GetPresetData, &::HapticPresetSO::GetPresetData, void, HapticPresetSO* self, float& strength, float& duration, float& frequency) {
    
    // 1. Run the base game logic first so the engine remains stable
    HapticPresetSO_GetPresetData(self, strength, duration, frequency);
    
    // 2. Apply our multiplier shift mid-air
    strength *= hapticForceMultiplier;
    if (strength > 1.0f) strength = 1.0f; // Cap at physical hardware limit
    
    duration *= 1.2f; // Elongate the vibration wave slightly for a thicker feel
}

// Setup mod metadata for the modern loader framework
extern "C" void setup(CModInfo& info) {
    info.id = "betterhaptics";
    info.version = "1.0.0";
}

// Intercept the engine memory loop right at game boot
extern "C" void load() {
    il2cpp_functions::Init();
    
    // Bind our custom scale logic directly into live game RAM
    INSTALL_HOOK(logger, HapticPresetSO_GetPresetData);
}

