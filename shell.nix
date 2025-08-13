{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Build tools
    gcc
    clang
    cmake
    gnumake
    pkg-config
    
    # OpenGL and graphics
    glfw
    mesa
    libGL
    
    # ImGui dependencies
    freetype
    
    # Additional graphics libraries
    glm
    stb
    
    # Development tools
    git
    gdb
    
  ] ++ pkgs.lib.optionals pkgs.stdenv.isDarwin [
    # macOS specific
    pkgs.darwin.apple_sdk.frameworks.OpenGL
    pkgs.darwin.apple_sdk.frameworks.Cocoa
    pkgs.darwin.apple_sdk.frameworks.IOKit
    pkgs.darwin.apple_sdk.frameworks.CoreVideo
    pkgs.darwin.apple_sdk.frameworks.CoreFoundation
  ] ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
    # Linux specific
    xorg.libX11
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXi
    libxkbcommon
    wayland
    wayland-protocols
  ];

  shellHook = ''
    echo "🎮 OpenGL3 + GLFW + GLAD + ImGui development environment loaded"
    echo "Platform: ${if pkgs.stdenv.isDarwin then "macOS" else "Linux"}"
    
    # Set up pkg-config paths
    export PKG_CONFIG_PATH="${pkgs.glfw}/lib/pkgconfig:${pkgs.freetype}/lib/pkgconfig:$PKG_CONFIG_PATH"
    
    # OpenGL setup
    ${pkgs.lib.optionalString pkgs.stdenv.isDarwin ''
      echo "🍎 macOS OpenGL framework available"
      export MACOSX_DEPLOYMENT_TARGET=10.15
    ''}
    
    ${pkgs.lib.optionalString pkgs.stdenv.isLinux ''
      echo "🐧 Linux OpenGL libraries available"
      export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [
        pkgs.libGL
        pkgs.glfw
        pkgs.xorg.libX11
        pkgs.xorg.libXcursor
        pkgs.xorg.libXrandr
        pkgs.xorg.libXinerama
        pkgs.xorg.libXi
      ]}:$LD_LIBRARY_PATH"
    ''}
    
    echo "GLAD available for OpenGL function loading! ⚡"
    echo "Ready to build your VacC app! 🚀"
  '';
}