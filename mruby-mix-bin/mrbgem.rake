MRuby::Gem::Specification.new('mruby-mix-bin') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime binaries}
  spec.bins        = ['mix']

  spec.add_dependency 'mruby-mix'
  spec.add_dependency 'mruby-compiler', :core => 'mruby-compiler'
end
