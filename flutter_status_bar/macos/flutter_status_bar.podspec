#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_status_bar.podspec' to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_status_bar'
  s.version          = '0.0.1'
  s.summary          = 'Status bar for flutter desktop.'
  s.description      = <<-DESC
  Status bar for flutter desktop.
                       DESC
  s.homepage         = 'https://github.com/RealCoolSnow/flutter_status_bar_plugin'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'CoolSnow' => 'coolsnow2020@gmail.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'

  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
end
