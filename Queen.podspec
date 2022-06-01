Pod::Spec.new do |s|
  s.name             = 'Queen'
  s.version          = '2.1.0-official-full'
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
  s.resources = 'Queen/Libs/queen.framework/mnn.metallib'
  s.vendored_frameworks = 'Queen/Libs/queen.framework', 'Queen/Libs/opencv2.framework'
  s.user_target_xcconfig = {"ENABLE_BITCODE" => "NO"}
end
