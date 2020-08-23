MRuby::Gem::Specification.new('mruby-mix') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime}

  spec.add_dependency 'mruby-pluggaloid', :github => 'shibafu528/mruby-pluggaloid', :branch => 'mruby'
end
