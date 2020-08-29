MRuby::Gem::Specification.new('mruby-mix') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime}

  spec.add_dependency 'mruby-pluggaloid', :github => 'shibafu528/mruby-pluggaloid', :branch => 'mruby'
  spec.add_dependency 'mruby-diva', :github => 'shibafu528/mruby-diva', :branch => 'mruby'
  spec.add_dependency 'mruby-delayer-deferred', :github => 'shibafu528/mruby-delayer-deferred', :branch => 'mruby'
  spec.add_dependency 'mruby-singleton', :mgem => 'mruby-singleton'
  spec.add_dependency 'mruby-yaml', :mgem => 'mruby-yaml'
end
