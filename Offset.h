Tools::Hook((void*)(uintptr_t)GetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("CameraSystem"), OBFUSCATE("LateUpdate"), 0),
            (void*)LateUpdate_CameraSystem_Hook, (void**)&_LateUpdate_CameraSystem);
