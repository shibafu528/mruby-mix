MRuby::Gem::Specification.new('mruby-mikutime-bin') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime binaries}
  spec.bins        = ['mikutime']

  spec.add_dependency 'mruby-mikutime'
  spec.add_dependency 'mruby-compiler', :core => 'mruby-compiler'
end
