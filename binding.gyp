{
'variables': {
    'module_name%': 'node-miniaudio',
    'module_path%': 'lib'
  },
    "targets": [
        {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
      },
        {
        'target_name': '<(module_name)',
        "sources": ["miniaudio.cpp"],
        'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
        'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
        "defines": ["MINIAUDIO_IMPLEMENTATION"],
        'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
       }
    }]
}