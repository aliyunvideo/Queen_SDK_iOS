Pod::Spec.new do |s|
  s.name             = 'Queen'
  s.version          = '1.1.0-official-pro'
  s.summary          = 'Aliyun Queen library'
  s.description      = <<-DESC
Aliyun Render And Compute Everything Engine.
                       DESC
  s.homepage         = 'https://github.com/aliyunvideo/Queen_SDK_iOS'
  s.license          = { :type => "Apache License, Version 2.0", :file => "LICENSE" }
  s.author           = { 'jiancong.zjc' => 'jiancong.zjc@alibaba-inc.com' }
  s.source           = { :git => 'https://github.com/aliyunvideo/Queen_SDK_iOS.git', :tag => s.version.to_s }
  s.ios.deployment_target = '9.0'
  s.requires_arc = true
  s.source_files = 'Queen/Libs/queen.framework/Headers/*'
  s.resources = 'Queen/Libs/Face3D.framework/face3d_res.Bundle', 'Queen/Libs/queen.framework/queen-ios.Bundle'
  s.vendored_frameworks = 'Queen/Libs/queen.framework', 'Queen/Libs/Face3D.framework', 'Queen/Libs/opencv2.framework', 'Queen/Libs/FaceDetection.framework', 'Queen/Libs/MNN.framework', 'Queen/Libs/pixelai.framework'
  s.frameworks = 'Metal', 'Accelerate', 'QuartzCore', 'OpenGLES', 'CoreMedia', 'CoreMotion', 'CoreImage', 'Foundation', 'AssetsLibrary', 'CoreGraphics', 'CoreVideo'
  s.weak_frameworks = 'MetalPerformanceShaders'
  s.libraries = 'c++', 'compression'
  s.user_target_xcconfig = {"ENABLE_BITCODE" => "NO"}
end
