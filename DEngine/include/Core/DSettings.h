#ifndef DEV_SETTINGS_H
#define DEV_SETTINGS_H

namespace dev
{
  static float  clearColor[4]     = { 0.6f, 0.6f, 1.0f, 1.0f };

  #define DEFAULT_LOG_FILE        L"DEngine.log"
  #define DEFAULT_VS_FUNCTION     "vs_main"
  #define DEFAULT_CS_FUNCTION     "cs_main"
  #define DEFAULT_PS_FUNCTION     "ps_main"
  #define DEFAULT_SPEED           0.01f
  #define DEFAULT_SENS            0.1f
  #define DEFAULT_NEAR            1.0f
  #define DEFAULT_FAR             10000.0f

  //passes num
  #define BACKGROUND_PASS_NUM     INT_MAX
  #define DEFERRED_PASS_NUM       INT_MAX - 1
}

#endif