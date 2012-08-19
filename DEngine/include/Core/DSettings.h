#ifndef DEV_SETTINGS_H
#define DEV_SETTINGS_H

namespace dev
{
  static float            clearColor[4]        = { 0.6f, 0.6f, 1.0f, 1.0f };
  static const wchar_t*   logFilename          = L"DEngine.log";
  static const char*      defaultVSfunction    = "vs_main";
  static const char*      defaultPSfunction    = "ps_main";
  static const float      defaultSpeed         = 0.01f;
  static const float      defaultSens          = 0.1f;
  static const float      defaultNear          = 0.01f;
  static const float      defaultFar           = 25000.0f;
}

#endif