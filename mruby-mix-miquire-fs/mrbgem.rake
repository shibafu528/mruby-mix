MRuby::Gem::Specification.new('mruby-mix-miquire-fs') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime - Miquire for file system}

  spec.add_dependency 'mruby-mix'
  spec.add_dependency 'mruby-require', :mgem => 'mruby-require'
  spec.add_dependency 'mruby-dir', :mgem => 'mruby-dir'
end
